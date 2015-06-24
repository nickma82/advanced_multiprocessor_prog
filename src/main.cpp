#include <stdio.h>

#include <iostream>
#include <thread>
#include <vector>
#include <set>
#include <stdlib.h>
#include <algorithm>
#include <memory>
#include <functional>
#include <chrono>

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

struct ThreadIOData {
	/* input data */
	AMPSet 			&rset;	//the multiThreaded target set
	const size_t	n;		//iterations count

	/* output data */
	std::vector<int> 	resultValues;	//for duplications checks only
	ValueStore 			valueStore;		//for timer measurements

	/*
	 * runManipulator has to decide whether another value should be
	 * added/removed/checked *and* has to manipulat the next value i
	 * which is going to be processed.
	 * @var[in] i 	reference to the runtime variable which is going to be
	 * 					added next
	 * @return 		true if another value should be added, false otherwise
	 */
	std::function<bool(int&)> runManipulator;

	ThreadIOData(AMPSet &rset, const size_t iterations) :
			rset(rset), n(iterations) {

		//increments i within every call and returns false if (i>=this->n)
		auto stdRunManipulator = [iterations](int &i) {i++; return (i<(int)iterations); };
		runManipulator = stdRunManipulator;
	};
	ThreadIOData(AMPSet &rset, const size_t n, std::function<bool(int&)> runManipulator) :
			rset(rset), n(n), runManipulator(runManipulator) {};
};


class SetTestActions {
public:
	/**
	 * removes or adds a block of data depending on the operationType.
	 */
	static void addRemove(ThreadIOData *ioData, const TimerType operationType) {
		if (operationType!=ID_INSERT && operationType!=ID_REMOVE)
			throw new std::invalid_argument("operation type must be either ID_INSERT or ID_REMOVE but was:");

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

	static bool condition_runUntilTime(int &i) {
		return false;
	}

	static void addRemoveUntil(ThreadIOData *ioData, const TimerType operationType) {
		if (operationType!=ID_INSERT && operationType!=ID_REMOVE)
			throw new std::invalid_argument("operation type must be either ID_INSERT or ID_REMOVE but was:");

		auto timer = StartStopTimer(operationType);

		int i = -1;
		while( ioData->runManipulator(i) ) {
			int rc = 0;
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

	static void containsUntil(ThreadIOData *ioData, const bool expected) {
		auto timer = StartStopTimer(ID_CONTAINS);

		int i = -1;
		while( ioData->runManipulator(i) ) {
			timer.time_start();
			auto contains = ioData->rset.contains(i);
			timer.time_stop();
			ioData->valueStore.addTimeMeasurement(timer);

			if(contains == expected)
				continue;

			std::cout << "wrong contain (until)!!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
};

/**
 * Calles the explicit SetTestAction
 */
class SetBenchmarkOperations {
	AMPSet &set;
	const size_t threadCount;
	const int iterations;

	/**
	 * This is *not* thread safe
	 *
	 * Checks for duplicates and executes exit(1)
	 * if any where found
	 */
	void _checkForDuplicates(std::vector<ThreadIOData> &ioDataMultithread) {
		//check if every element was only added once
		//  runtime is O(n*threadCount) which is linear
		std::set<int> checkDuplicates1;

		for(size_t i=0; i<threadCount; ++i) {
			std::vector<int> vector = ioDataMultithread[i].resultValues;

			while(!vector.empty()) {
				long item = vector.back();
				vector.pop_back();
				auto result = checkDuplicates1.insert(item);
				if(!result.second) {
					std::cout << "duplicate add!! \n";
					exit(1);
				}
			}
		} //end for loop
	}

public:
	/*
	 * The constructor takes variables needed by every method in this class
	 */
	SetBenchmarkOperations(AMPSet &targetSet, const size_t threadCount, const int iterations) :
		set(targetSet), threadCount(threadCount), iterations(iterations) {
		std::cout << "Initializing SetBenchmarkOperations with " << threadCount << " threads, with " << iterations << "iterations" << std::endl;
	}

	void basicFunctionality() {
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


	void multithreadAddOnly() {
		std::cout << "going to fire up " << threadCount << " threads, with " << iterations << "iterations" << std::endl;
		//run add only test on multiple threads
		std::vector<ThreadIOData> insertIOData(threadCount, ThreadIOData(set, iterations));
		std::vector<std::thread> threadVector1(threadCount);
		for(size_t i=0; i<threadCount; ++i) {
			threadVector1[i] = std::thread( SetTestActions::addRemove, &insertIOData[i], ID_INSERT );
		}

		ValueAnalyser insert_analyser;
		for(size_t i=0; i<threadCount; ++i) {
			threadVector1.at(i).join();

			//feed ValueStore
			insert_analyser.addValueStore(insertIOData[i].valueStore, i);
		}
		insert_analyser.outputToFile("/tmp/results_inserting");

		_checkForDuplicates(insertIOData);
	} //multithreadAddOnly(..)

	/**
	 * Triggers a multithreaded add proces in which every thread runs a second
	 * @note threads are *not* startet all at the same time
	 */
	void multithreadedAddOneSecond() {
		//create the runCondition
		const uint8_t runTimeSeconds = 5;
		auto stopTime =
				std::chrono::system_clock::now() + std::chrono::seconds(runTimeSeconds);
		std::function<bool(int&)> runCondition = [stopTime](int &i) {
			i++;
			return (std::chrono::system_clock::now() < stopTime);
		};


		/* same as in multithreadAddonly */
		std::vector<ThreadIOData> insertIOData(threadCount, ThreadIOData(set, iterations, runCondition));
		std::vector<std::thread> threadVector1(threadCount);
		for(size_t i=0; i<threadCount; ++i) {
			threadVector1[i] = std::thread( SetTestActions::addRemoveUntil, &insertIOData[i], ID_INSERT );
		}

		ValueAnalyser insert_analyser;
		for(size_t i=0; i<threadCount; ++i) {
			threadVector1.at(i).join();

			//feed ValueStore
			insert_analyser.addValueStore(insertIOData[i].valueStore, i);
		}
		insert_analyser.outputToFile("/tmp/results_inserting");

		_checkForDuplicates(insertIOData);
	}

	/*
	 * check if the set responses with @var{expected} to a contains check of
	 * the values [0,1,..,threadCount]
	 */
	void multithreadContains(const bool expected = true) {
		std::vector<ThreadIOData> containsIOData(threadCount, ThreadIOData(set, iterations));
		std::vector<std::thread> threadVector2(threadCount);
		for(size_t i=0; i<threadCount; ++i) {
			threadVector2.at(i) = std::thread ( SetTestActions::containsUntil, &containsIOData[i], expected);
		}

		ValueAnalyser contains_analyser;
		for(size_t i=0; i<threadCount; ++i) {
			threadVector2.at(i).join();

			//feed ValueStore
			contains_analyser.addValueStore(containsIOData[i].valueStore, i);
		}
		contains_analyser.outputToFile("/tmp/results_contains");
	}



	void multithreadRemoveAll() {
		//remove all elements again
		std::vector<std::thread> threadVector3(threadCount);
		std::vector<ThreadIOData> remove_ioData(threadCount, ThreadIOData(set, iterations));
		for(size_t i=0; i<threadCount; ++i) {
			threadVector3[i] = std::thread( SetTestActions::addRemove, &remove_ioData[i], ID_REMOVE );
		}

		ValueAnalyser remove_analyser;
		for(size_t i=0; i<threadCount; ++i) {
			threadVector3.at(i).join();

			//feed ValueStore
			remove_analyser.addValueStore(remove_ioData[i].valueStore, i);
		}
		remove_analyser.outputToFile("/tmp/results_removing");

		_checkForDuplicates(remove_ioData);
	}
};

void benchmarkBareRun(AMPSet &set, const size_t threadCount, const int iterations) {
	SetBenchmarkOperations setBenchmarkOperations(set, threadCount, iterations);
	setBenchmarkOperations.basicFunctionality();

	setBenchmarkOperations.multithreadAddOnly();
	setBenchmarkOperations.multithreadContains(true);

	setBenchmarkOperations.multithreadRemoveAll();
	setBenchmarkOperations.multithreadContains(false);

	std::cout << "test successfull \n";
}

void benchmarkRunUntilTime(AMPSet &set, const size_t threadCount, const int iterations) {
	//add testData: single threaded - without benchmark
	//contains benchmark : multithreaded
	//remove benchmark: remove multtith
	//removeAll : remove single threaded - without benchmark

	//add multithreaded - with benchmark
	SetBenchmarkOperations setBenchmarkOperations(set, threadCount, iterations);
	setBenchmarkOperations.multithreadedAddOneSecond();
}

int main (int argc, char **argv) {
	CommandLineOptions cmdOption;
	int rc = cmdOption.parse_args(argc, argv);
	if (rc < 0)
		return EXIT_FAILURE;

	// get user values. if not set, set to default
	const int threadCount =  (int)cmdOption.getValue("threadCount")  ? cmdOption.getValue("threadCount")  : 50;
	const int repeatCycles = (int)cmdOption.getValue("repeatCycles") ? cmdOption.getValue("repeatCycles") : 1000;
	//options are: FGL, OS, LS, LBS, REF
	const std::string implementation = cmdOption.getValue("implementation");

	// Set* selector
	std::unique_ptr<AMPSet> targetSet = nullptr;
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
	if(targetSet.get() == nullptr) {
		std::cout << "No targetSet set, bye" << std::endl;
		return EXIT_SUCCESS;
	}

	if( (bool)cmdOption.getValue("benchmarkTimeout") ) {
		benchmarkRunUntilTime(*targetSet, threadCount, repeatCycles);
	} else {
		std::cout << "starting implementation:" << implementation << std::endl;
		std::cout << "calling benchmarkBareRun " << threadCount << " threads, with " << repeatCycles << "iterations" << std::endl;
		benchmarkBareRun(*targetSet, threadCount, repeatCycles);
	}

	return EXIT_SUCCESS;
}
