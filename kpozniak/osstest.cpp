/*
 * referencetest.cpp
 *
 * Advanced Multiprocessor Programming
 * Author: kpozniak
 *
 *	compile with:
 *  g++ -std=c++11 -Wall -O3 AMPReferenceSet.cpp referencetest.cpp -o test
 *
 */


#include <iostream>
#include <thread>
#include "OptimisticSynchronizationSet.h"

void test(int t, AMPSet *rs, volatile bool *start)
{
			
	while(!*start);
	
	long i=0;
	
	while(i<100) {
		bool result = rs->add(i);
		std::string str ("thread ");
		str+=std::to_string(t);
		str+=" add ";
		str+=std::to_string(i);
		str+=" ";
		str+=std::to_string(result);
		std::cout << str+"\n";
		i++;
	}
	
	
}

int main() 
{
	
	//run some basic functionality tests
	OptimisticSynchronizationSet rs1;
	
	
	std::cout << std::boolalpha;   
	std::cout << "add " << rs1.add(5) <<"=true\n";
	std::cout << "add " << rs1.add(5) <<"=false\n";
	std::cout << "add " << rs1.add(6) <<"=true\n";
	std::cout << "add " << rs1.add(7) <<"=true\n";
	std::cout << "add " << rs1.add(8) <<"=true\n";
	std::cout << "add " << rs1.add(1) <<"=true\n";
	std::cout << "add " << rs1.add(8) <<"=false\n";
	std::cout << "add " << rs1.add(1) <<"=false\n";
	
	std::cout << "contains " << rs1.contains(1) <<"=true\n";
	
	std::cout << "remove " << rs1.remove(1) <<"=true\n";
	std::cout << "remove " << rs1.remove(1) <<"=false\n";
	
	std::cout << "contains " << rs1.contains(1) <<"=false\n";
	
	
	//run 4 test threads
	
	OptimisticSynchronizationSet rs2;

	bool start = false;
	
	std::thread first (test, 1, &rs2, &start); 
	std::thread second (test, 2, &rs2, &start); 
	std::thread third (test, 3, &rs2, &start); 
	std::thread fourth (test, 4, &rs2, &start); 
	
	start = true;
	
	first.join();
	second.join();
	third.join();
	fourth.join();
	
	
	return 0;
}