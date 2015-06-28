/*
* osstest.cpp
*
* Advanced Multiprocessor Programming
* Author: kpozniak
*
*
*/

#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <stdlib.h>

#include "OptimisticSynchronizationSet.h"
#include "FineGrainedLockingSet.h"
#include "LazySynchronizationSet.h"
#include "LockFreeSet.h"
#include "AMPReferenceSet.h"
#include "AMPSet.h"
#include "cmdlineOptions.h"

void addTest(AMPSet *set, int n, std::vector<long> *vec)
{	
	long i=0;
	
	while(i<n) {
		long j=0;
		while(j<10) {
			if(set->add(i)) vec->push_back(i);
			j++;
		}
		i++;
	}	
	
}

void containsTest(AMPSet *set, int n, bool expected)
{
	long i=0;
	
	while(i<n) {
		
		if(set->contains(i) != expected) {
			std::cout << "wrong contain!! \n";
			exit(EXIT_FAILURE);
		}
		
		i++;
	}
	
}

void removeTest(AMPSet *set, int n, std::vector<long> *vec)
{
	long i=0;
	
	while(i<n) {
		long j=0;
		while(j<10) {
			if(set->remove(i)) vec->push_back(i);
			j++;
		}
		i++;
	}
	
}

void mixedTest(AMPSet *set, long id, int n) {
	
	int i = 0;
	
	while(i<n) {
		
		if(set->add(id) != true) {
			std::cout << "wrong add!! \n";
			exit(EXIT_FAILURE);
		}
		if(set->contains(id) != true) {
			std::cout << "wrong contains!! \n";
			exit(EXIT_FAILURE);
		}
		if(set->remove(id) != true) {
			std::cout << "wrong remove!! \n";
			exit(EXIT_FAILURE);
		}
		if(set->contains(id) != false) {
			std::cout << "wrong contains!! \n";
			exit(EXIT_FAILURE);
		}
		i++;
	}
	
}

void test(AMPSet *set, int threads, int iterations) {
	
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
	
	std::vector<std::thread> threadVector(threads);
	std::vector<std::vector<long>> checkVectors1(threads, std::vector<long>());
	
	for(int i=0; i<threads; i++) {
		threadVector.at(i) = std::thread (addTest, set, iterations, &checkVectors1.at(i));
	}
	
	for(int i=0; i<threads; i++) {
		threadVector.at(i).join();
	}
	
	//check if every element was only added once
	
	std::set<long> checkDuplicates1;
	
	for(int i=0; i<threads; i++) {
		std::vector<long> vector = checkVectors1.at(i);
		while(!vector.empty()) {
			long item = vector.back();
			vector.pop_back();
			auto result = checkDuplicates1.insert(item);
			if(!result.second) {
				std::cout << "duplicate add!! " << item << " was added twice!" << std::endl;
				exit(1);
			}
		}
	}
	
	
	//check if the right elements are contained in the set
	
	for(int i=0; i<threads; i++) {
		threadVector.at(i) = std::thread (containsTest, set, iterations, true);
	}
	
	for(int i=0; i<threads; i++) {
		threadVector.at(i).join();
	}
	
	
	//remove all elements again
	
	std::vector<std::vector<long>> checkVectors3(threads, std::vector<long>());
	
	for(int i=0; i<threads; i++) {
		threadVector.at(i) = std::thread (removeTest, set, iterations, &checkVectors3.at(i));
	}
	
	for(int i=0; i<threads; i++) {
		threadVector.at(i).join();
	}
	
	//check if every element was only removed once
	
	std::set<long> checkDuplicates3;
	
	for(int i=0; i<threads; i++) {
		std::vector<long> vector = checkVectors3.at(i);
		while(!vector.empty()) {
			long item = vector.back();
			vector.pop_back();
			auto result = checkDuplicates3.insert(item);
			if(!result.second) {
				std::cout << "wrong remove!! " << item << " was removed twice!" << std::endl;
				exit(1);
			}
		}
	}
	
	//check if the set is now empty
	
	for(int i=0; i<threads; i++) {
		threadVector.at(i) = std::thread (containsTest, set, iterations, false);
	}
	
	for(int i=0; i<threads; i++) {
		threadVector.at(i).join();
	}
	
	for(int i=0; i<threads; i++) {
		threadVector.at(i) = std::thread (mixedTest, set, i, iterations);
	}
	
	for(int i=0; i<threads; i++) {
		threadVector.at(i).join();
	}
	
	std::cout << "test successfull \n";
	
	
}

int main(int argc, char **argv) {
	
	CommandLineOptions cmdOption;
	int rc = cmdOption.parse_args(argc, argv);
	if (rc < 0)
		return -1;
	
	int n, t;
	
	if((int)cmdOption.getValue("threadCount")) {
		t = cmdOption.getValue("threadCount");
	} else {
		t = 10;
	}
	
	if((int)cmdOption.getValue("repeatCycles")) {
		n = cmdOption.getValue("repeatCycles");
	} else {
		n = 100;
	}
	
	std::cout << "testing FineGrainedLockingSet..." << std::endl;
	FineGrainedLockingSet fgls1;
	test(&fgls1, t, n);
	
	std::cout << "testing OptimisticSynchronizationSet..." << std::endl;
	OptimisticSynchronizationSet oss1;
	test(&oss1, t, n);	
	
	std::cout << "testing LazySynchronizationSet..." << std::endl;
	LazySynchronizationSet lss1;
	test(&lss1, t, n);
	
	std::cout << "testing LockFreeSet..." << std::endl;
	LockFreeSet lfs1;
	test(&lfs1, t, n);
	
	return 0;
}