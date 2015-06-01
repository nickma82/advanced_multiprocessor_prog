#include "ValueAnalyser.h"


void ValueAnalyser::addValueStore(const ValueStore &store) {
		addValueStore(store, store.threadID);
	}

void ValueAnalyser::addValueStore(const ValueStore &store, const unsigned int threadID) {
		std::cout << "adding timerStor with:" << (uint16_t)store.getStoredValueCount()
				<< " values, from threadID:" << threadID << std::endl;
		for(auto each: store.measuredValues) {
			auto newEl = ThreadTimerValue(each, threadID);
			_threadedTimerValues.push_back(newEl);
		}
	}

void ValueAnalyser::outputToFile(const std::string &pathToFile) {
	std::cout << "going to open file '" << pathToFile
			<< "' to write '" << _threadedTimerValues.size()
			<< "' values to it" << std::endl;

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

	myfile.close();
}
