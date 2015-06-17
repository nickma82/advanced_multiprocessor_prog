#include "ValueAnalyser.h"
#include <ostream>
#include <iostream>
#include <stdexcept>

void ValueAnalyser::addValueStore(const ValueStore &store) {
	if(!store.threadID)
		throw new std::runtime_error("tried to obtain the threadID from the ValueStore, but it was zero");
	addValueStore(store, store.threadID);
}

void ValueAnalyser::addValueStore(const ValueStore &store, const unsigned int threadID) {
		std::cout << "add(" << (uint16_t)store.getStoredValueCount()
				<< " from thr:" << threadID << ") ";

		for(auto each: store.measuredValues) {
			auto newEl = ThreadTimerValue(each, threadID);
			_threadedTimerValues.push_back(newEl);
		}
	}

void ValueAnalyser::outputToFile(const std::string &pathToFile) {
	std::cout << "\n" << "going to open file '" << pathToFile
			<< "' to write '" << _threadedTimerValues.size()
			<< "' values to it" << std::endl;

	// opening the file
	std::ofstream myfile;
	myfile.open(pathToFile);

	// print first line
	myfile << std::setw(10) << std::setfill(' ') << "Thread"
			<< std::setw(10) << std::setfill(' ') << "Duration"
			<< std::setw(10) << std::setfill(' ') << "TimerType"
			<< std::endl;

	// print out all other lines
	for(auto each: _threadedTimerValues) {
		myfile << each << std::endl;
	}

	// saving to the file
	myfile.close();
}
