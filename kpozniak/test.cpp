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
#include "AMPReferenceSet.h"
#include "AMPSet.h"



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

void containsTest(AMPSet *rs, int n)
{
	long i=0;
	
	while(i<n) {
		
		if(!rs->contains(i)) {
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
	
	std::vector<std::thread> threadVector1(threads);
	std::vector<std::vector<long>> checkVectors1(threads, std::vector<long>());
	
	for(int i=0; i<threads; i++) {
		threadVector1.at(i) = std::thread (addTest, set, iterations, &checkVectors1.at(i));
	}
	
	for(int i=0; i<threads; i++) {
		threadVector1.at(i).join();
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
				std::cout << "duplicate add!! \n";
				exit(1);
			}
		}
	}
	
	
	//check if the right elements are contained in the set
	
	std::vector<std::thread> threadVector2(threads);
	
	for(int i=0; i<threads; i++) {
		threadVector2.at(i) = std::thread (containsTest, set, iterations);
	}
	
	for(int i=0; i<threads; i++) {
		threadVector2.at(i).join();
	}
	
	
	//remove all elements again
	
	std::vector<std::thread> threadVector3(threads);
	std::vector<std::vector<long>> checkVectors3(threads, std::vector<long>());
	
	for(int i=0; i<threads; i++) {
		threadVector3.at(i) = std::thread (removeTest, set, iterations, &checkVectors3.at(i));
	}
	
	for(int i=0; i<threads; i++) {
		threadVector3.at(i).join();
	}
	
	//check if every element was only added once
	
	std::set<long> checkDuplicates3;
	
	for(int i=0; i<threads; i++) {
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
		
	std::cout << "test successfull \n";
	
	
}

int main() 
{
	
	AMPReferenceSet rs1;
	test(&rs1, 50, 1000);
	
	FineGrainedLockingSet fgls1;
	test(&fgls1, 50, 1000);
	
	OptimisticSynchronizationSet oss1;
	test(&oss1, 50, 1000);	
	
	
	return 0;
}