#include <stdio.h>

#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <stdlib.h>
#include <algorithm>
#include <memory>

#include "ValueAnalyser.h"
#include "TimeMeasurement.h"
#include "TimeValues.h"
#include "ValueStore.h"
#include "OptimisticSynchronizationSet.h"
#include "LazySynchronizationSet.h"
#include "LockFreeSet.h"
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
	size_t		n; //iterations count

	// output data
	std::vector<int> 	resultValues; //for duplications checks only
	ValueStore 			valueStore;  //for timer measurements

	ThreadIOData(AMPSet &rset, size_t n) :
		rset(rset), n(n) {};
};


class SetTestActions {
public:
	/**
	 * removes or adds a block of data depending on the operationType.
	 */
	static void addRemove(ThreadIOData *ioData, const TimerType operationType) {
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
				ioData->valueStore.addTimeMeasurement(timer);

				/* evaluation part, save to result values */
				if(rc) {
					ioData->resultValues.push_back(i);
				}
			}
		}
	}


	static void contains(ThreadIOData *ioData, const bool expected) {
		auto timer = StartStopTimer(ID_CONTAINS);

		//@note no need to fill ioData->resultValues because we arent going to
		// check for duplicated contains-checks
		for(size_t i=0; i < ioData->n; ++i) {
			timer.time_start();
			auto contains = ioData->rset.contains(i);
			timer.time_stop();
			ioData->valueStore.addTimeMeasurement(timer);

			if(contains == expected)
				continue;

			std::cout << "wrong contain!! \n";
			exit(EXIT_FAILURE);
		}

	}
};

class SetBenchmarkOperations {
public:
	static void basicFunctionality(AMPSet &set, const size_t threads, const int) {
		//run some basic functionality tests on one thread
		if(set.add(5)!=true) exit(EXIT_FAILURE);
		if(set.add(5)!=false) exit(EXIT_FAILURE);
		if(set.add(6)!=true) exit(EXIT_FAILURE);
		if(set.add(7)!=true) exit(EXIT_FAILURE);
		if(set.add(8)!=true) exit(EXIT_FAILURE);
		if(set.add(1)!=true) exit(EXIT_FAILURE);
		if(set.add(8)!=false) exit(EXIT_FAILURE);
		if(set.add(1)!=false) exit(EXIT_FAILURE);

		if(set.contains(1)!=true) exit(EXIT_FAILURE);

		if(set.remove(1)!=true) exit(EXIT_FAILURE);
		if(set.remove(1)!=false) exit(EXIT_FAILURE);

		if(set.contains(1)!=false) exit(EXIT_FAILURE);
	}

	static void multithreadAddOnly(AMPSet &set, const size_t threads, const int iterations) {
		//run add only test on multiple threads
		ValueAnalyser insert_analyser;
		std::vector<ThreadIOData> insertIOData(threads, ThreadIOData(set, iterations));
		std::vector<std::thread> threadVector1(threads);

		for(size_t i=0; i<threads; ++i) {
			threadVector1[i] = std::thread( SetTestActions::addRemove, &insertIOData[i], ID_INSERT );
		}
		for(size_t i=0; i<threads; ++i) {
			threadVector1.at(i).join();

			//feed ValueStore
			insert_analyser.addValueStore(insertIOData[i].valueStore, i);
		}
		insert_analyser.outputToFile("/tmp/results_inserting");


		//check if every element was only added once
		//  runtime is O(n*threads) which is linear
		std::set<int> checkDuplicates1;

		for(size_t i=0; i<threads; ++i) {
			std::vector<int> vector = insertIOData[i].resultValues;

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
	} //multithreadAddOnly(..)

	/*
	 * check if the set responses with @var{expected} to a contains check of
	 * the values [0,1,..,threads]
	 */
	static void multithreadContains(AMPSet &set, const size_t threads, const int iterations, const bool expected = true) {
		std::vector<ThreadIOData> containsIOData(threads, ThreadIOData(set, iterations));
		std::vector<std::thread> threadVector2(threads);

		for(size_t i=0; i<threads; ++i) {
			threadVector2.at(i) = std::thread ( SetTestActions::contains, &containsIOData[i], expected);
		}

		ValueAnalyser contains_analyser;
		for(size_t i=0; i<threads; ++i) {
			threadVector2.at(i).join();

			//feed ValueStore
			contains_analyser.addValueStore(containsIOData[i].valueStore, i);
		}
		contains_analyser.outputToFile("/tmp/results_contains");
	}

	static void multithreadRemoveAll(AMPSet &set, const size_t threads, const int iterations) {
		//remove all elements again
		std::vector<std::thread> threadVector3(threads);
		std::vector<ThreadIOData> remove_ioData(threads, ThreadIOData(set, iterations));

		for(size_t i=0; i<threads; ++i) {
			threadVector3[i] = std::thread( SetTestActions::addRemove, &remove_ioData[i], ID_REMOVE );
		}

		ValueAnalyser remove_analyser;
		for(size_t i=0; i<threads; ++i) {
			threadVector3.at(i).join();

			//feed ValueStore
			remove_analyser.addValueStore(remove_ioData[i].valueStore, i);
		}
		remove_analyser.outputToFile("/tmp/results_removing");

		//check if every element was only removed once
		std::set<int> checkDuplicates3;

		for(size_t i=0; i<threads; ++i) {
			std::vector<int> vector = remove_ioData[i].resultValues;
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
	}
};

void test(AMPSet &set, const size_t threads, const int iterations) {

	SetBenchmarkOperations::basicFunctionality(set, threads, iterations);

	SetBenchmarkOperations::multithreadAddOnly(set, threads, iterations);
	SetBenchmarkOperations::multithreadContains(set, threads, iterations, true);

	SetBenchmarkOperations::multithreadRemoveAll(set, threads, iterations);
	SetBenchmarkOperations::multithreadContains(set, threads, iterations, false);


	std::cout << "test successfull \n";
}

int main (int argc, char **argv) {
	CommandLineOptions cmdOption;
	int rc = cmdOption.parse_args(argc, argv);
	if (rc < 0)
		return EXIT_FAILURE;

	// get user values. if not set, set to default
	const int threadCount =  cmdOption.getValue("threadCount")  ? cmdOption.getValue("threadCount")  : 50;
	const int repeatCycles = cmdOption.getValue("repeatCycles") ? cmdOption.getValue("repeatCycles") : 1000;
	//options are: FGL, OS, LS, LBS, REF
	const std::string implementation = cmdOption.getValue("implementation");

	// Set* selector
	std::unique_ptr<AMPSet> targetSet;
	if(implementation == "REF") {
		targetSet.reset(new AMPReferenceSet);
	} else if(implementation == "FGL") {
		targetSet.reset(new FineGrainedLockingSet);
	}  else if(implementation == "OS") {
		targetSet.reset(new OptimisticSynchronizationSet);
	}  else if(implementation == "LS") {
		targetSet.reset(new LazySynchronizationSet);
	} else if(implementation == "LF") {
		targetSet.reset(new LockFreeSet);
	} else {
		std::cout << "implementation parameter '" << implementation << "' not recognized" << std::endl;
		std::cout << "please add at least --implementation=<option>" << std::endl;
		cmdOption.printUsage();
	}

	// execute set tests
	if(targetSet.get() != nullptr) {
		std::cout << "starting implementation:" << implementation << std::endl;
		test(*targetSet, threadCount, repeatCycles);
	}



	return EXIT_SUCCESS;
}
