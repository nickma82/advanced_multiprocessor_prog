#include <stdio.h>

#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <stdlib.h>

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

void removeTest(AMPSet *rs, int n, std::vector<long> *vec)
{
	long i=0;

	while(i<n) {
		long j=0;
		while(j<10) {
			if(rs->remove(i)) vec->push_back(i);
			j++;
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


	//run add only test on multiple threads

	std::vector<std::thread> threadVector1(threads);
	std::vector<std::vector<long>> checkVectors1(threads, std::vector<long>());

	for(size_t i=0; i<threads; ++i) {
		threadVector1.at(i) = std::thread (addTest, set, iterations, &checkVectors1.at(i));
	}

	for(size_t i=0; i<threads; ++i) {
		threadVector1.at(i).join();
	}

	//check if every element was only added once

	std::set<long> checkDuplicates1;

	for(size_t i=0; i<threads; ++i) {
		std::vector<long> vector = checkVectors1.at(i);
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
	std::vector<std::vector<long>> checkVectors3(threads, std::vector<long>());

	for(size_t i=0; i<threads; ++i) {
		threadVector3.at(i) = std::thread (removeTest, set, iterations, &checkVectors3.at(i));
	}

	for(size_t i=0; i<threads; ++i) {
		threadVector3.at(i).join();
	}

	//check if every element was only added once

	std::set<long> checkDuplicates3;

	for(size_t i=0; i<threads; ++i) {
		std::vector<long> vector = checkVectors3.at(i);
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
	//const std::string implementation = cmdOption.getValue("implementation");

	std::cout << "starting AMPReferenceSet" << std::endl;
	AMPReferenceSet rs1;
	test(&rs1, threadCount, repeatCycles);

	std::cout << "starting FineGrainedLockingSet" << std::endl;
	FineGrainedLockingSet fgls1;
	test(&fgls1, threadCount, repeatCycles);

	std::cout << "starting OptimisticSynchronizationSet" << std::endl;
	OptimisticSynchronizationSet oss1;
	test(&oss1, threadCount, repeatCycles);



	return EXIT_SUCCESS;
}
