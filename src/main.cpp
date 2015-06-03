#include <stdio.h>

#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <stdlib.h>
#include <algorithm>

#include "ValueAnalyser.h"
#include "TimeMeasurement.h"
#include "TimeValues.h"
#include "ValueStore.h"
#include "OptimisticSynchronizationSet.h"
#include "FineGrainedLockingSet.h"
#include "AMPReferenceSet.h"
#include "AMPSet.h"

#include "cmdlineOptions.h" //command line parser



void addTest(AMPSet *rs, int n, std::vector<long> *vec)
{
	long i=0;

	while(i<n) {
		long j=0;
		while(j<10) {
			if(rs->add(i)) vec->push_back(i);
			j++;
		}
		i++;
	}

}

struct ThreadIOData {
	//input data
	AMPSet 		&rset;
	size_t		n;

	// output data
	std::vector<int> 	resultValues;
	ValueStore 			valueStore;

	ThreadIOData(AMPSet &rset, size_t n) :
		rset(rset), n(n) {};
};

/**
 * removes or adds a block of data depending on the operationType.
 */
void testAddRemoveBenchmarked(ThreadIOData *ioData, const TimerType operationType) {
//		AMPSet *rset, size_t n, std::vector<long> *output) {
	const size_t pseudoRandomValue = 10;
	/*
	 * Adds pseudoRandomValue times the same value
	 */
	bool rc = false;
	auto timer = StartStopTimer(operationType);

	for(size_t i=0; i<ioData->n; ++i) {
		for(size_t j=0; j<pseudoRandomValue; ++j) {

			timer.time_start();
			// differ between insert and remove
			if(operationType == ID_INSERT) {
				rc = ioData->rset.add(i);
			} else if (operationType == ID_REMOVE) {
				rc = ioData->rset.remove(i);
			}
			timer.time_stop();

			/* evaluation part */
			if(rc) {
				ioData->resultValues.push_back(i);
			}
			ioData->valueStore.addTimeMeasurement(timer);
		}
	}
}

void containsTest(AMPSet *rs, int n, bool expected)
{
	long i=0;

	while(i<n) {

		if(rs->contains(i) != expected) {
			std::cout << "wrong contain!! \n";
			exit(EXIT_FAILURE);
		}

		i++;
	}

}

void test(AMPSet *set, const size_t threads, const int iterations) {

	//run some basic functionality tests on one thread

	if(set->add(5)!=true) exit(EXIT_FAILURE);
	if(set->add(5)!=false) exit(EXIT_FAILURE);
	if(set->add(6)!=true) exit(EXIT_FAILURE);
	if(set->add(7)!=true) exit(EXIT_FAILURE);
	if(set->add(8)!=true) exit(EXIT_FAILURE);
	if(set->add(1)!=true) exit(EXIT_FAILURE);
	if(set->add(8)!=false) exit(EXIT_FAILURE);
	if(set->add(1)!=false) exit(EXIT_FAILURE);

	if(set->contains(1)!=true) exit(EXIT_FAILURE);

	if(set->remove(1)!=true) exit(EXIT_FAILURE);
	if(set->remove(1)!=false) exit(EXIT_FAILURE);

	if(set->contains(1)!=false) exit(EXIT_FAILURE);


//	//run add only test on multiple threads
	std::vector<std::thread> threadVector1(threads);
	std::vector<ThreadIOData> ioData(threads, ThreadIOData(*set, iterations));
	ValueAnalyser analyser_insert;

	for(size_t i=0; i<threads; ++i) {
		threadVector1[i] = std::thread( testAddRemoveBenchmarked, &ioData[i], ID_INSERT );
	}

	for(size_t i=0; i<threads; ++i) {
		threadVector1.at(i).join();

		//feed ValueStore
		analyser_insert.addValueStore(ioData[i].valueStore, i);
	}
	analyser_insert.outputToFile("/tmp/results_inserting");


	//check if every element was only added once
	std::set<int> checkDuplicates1;

	for(size_t i=0; i<threads; ++i) {
		std::vector<int> vector = ioData[i].resultValues;

		while(!vector.empty()) {
			long item = vector.back();
			vector.pop_back();
			auto result = checkDuplicates1.insert(item);
			if(!result.second) {
				std::cout << "duplicate add!! \n";
				exit(1);
			}
		}
	}


	//check if the right elements are contained in the set
	std::vector<std::thread> threadVector2(threads);

	for(size_t i=0; i<threads; ++i) {
		threadVector2.at(i) = std::thread (containsTest, set, iterations, true);
	}

	for(size_t i=0; i<threads; ++i) {
		threadVector2.at(i).join();
	}


	//remove all elements again

	std::vector<std::thread> threadVector3(threads);
	std::vector<ThreadIOData> ioData_rm(threads, ThreadIOData(*set, iterations));
	ValueAnalyser analyser_remove;

	for(size_t i=0; i<threads; ++i) {
		threadVector3[i] = std::thread( testAddRemoveBenchmarked, &ioData_rm[i], ID_REMOVE );
//		threadVector3.at(i) = std::thread (removeTest, set, iterations, &checkVectors3.at(i));
	}

	for(size_t i=0; i<threads; ++i) {
		threadVector3.at(i).join();

		//feed ValueStore
		analyser_remove.addValueStore(ioData_rm[i].valueStore, i);
	}
	analyser_remove.outputToFile("/tmp/results_removing");

	//check if every element was only added once
	std::set<int> checkDuplicates3;

	for(size_t i=0; i<threads; ++i) {
		std::vector<int> vector = ioData_rm[i].resultValues;
		while(!vector.empty()) {
			long item = vector.back();
			vector.pop_back();
			auto result = checkDuplicates3.insert(item);
			if(!result.second) {
				std::cout << "wrong remove!! \n";
				exit(1);
			}
		}
	}

	//check if the set is now empty

	std::vector<std::thread> threadVector4(threads);

	for(size_t i=0; i<threads; ++i) {
		threadVector4.at(i) = std::thread (containsTest, set, iterations, false);
	}

	for(size_t i=0; i<threads; ++i) {
		threadVector4.at(i).join();
	}

	std::cout << "test successfull \n";
}

int main (int argc, char **argv) {
	CommandLineOptions cmdOption;
	int rc = cmdOption.parse_args(argc, argv);
	if (rc < 0)
		return EXIT_FAILURE;

	const int threadCount =  cmdOption.getValue("threadCount")  ? cmdOption.getValue("threadCount")  : 50;
	const int repeatCycles = cmdOption.getValue("repeatCycles") ? cmdOption.getValue("repeatCycles") : 1000;
	//options are: FGL, OS, LS, LBS, REF
	const std::string implementation = cmdOption.getValue("implementation");

	if(implementation == "REF") {
		std::cout << "starting AMPReferenceSet" << std::endl;
		AMPReferenceSet rs1;
		test(&rs1, threadCount, repeatCycles);
	} else if(implementation == "FGL") {
		std::cout << "starting FineGrainedLockingSet" << std::endl;
		FineGrainedLockingSet fgls1;
		test(&fgls1, threadCount, repeatCycles);
	} else if(implementation == "OS") {
		std::cout << "starting OptimisticSynchronizationSet" << std::endl;
		OptimisticSynchronizationSet oss1;
		test(&oss1, threadCount, repeatCycles);
	} else {
		std::cout << "implementation parameter '" << implementation << "' not recognised" << std::endl;
		std::cout << "please add at least --implementation=<option>" << std::endl;
		cmdOption.printUsage();
	}





	return EXIT_SUCCESS;
}
