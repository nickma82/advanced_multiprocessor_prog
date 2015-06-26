#include "TimeMeasurement.h"

#include <fstream>
#include <ostream>
#include <iostream>
#include <stdexcept>

void StartStopTimer::time_start() {
	setMeasurementRunning(true);
	invalidateTimeSpan();

	_startTime = high_resolution_clock::now();
}

void StartStopTimer::time_stop() {
	auto tmpTimeNow = high_resolution_clock::now();

	timespan = duration_cast < DURATION_BASE > (tmpTimeNow - _startTime);
	setMeasurementRunning(false);
}

DURATION_BASE StartStopTimer::getTimeSpan() const {
	return timespan;
}

void StartStopTimer::invalidateTimeSpan() {
	timespan = INVALID_TIMESPAN;
}

void StartStopTimer::setMeasurementRunning(const bool &nextState) {
	/* consistency check */
	if(_measurementRunning == nextState)
		throw std::runtime_error(
				std::string("cant set the clock twice to the same value: ") +
				std::string(nextState?"1":"0") );

	_measurementRunning = nextState;
}

void StartStopTimer::outputToFile(const std::string &pathToFile) {
	std::cout << "\n" << "going to open file '" << pathToFile
				<< "' to write the timer value to it" << std::endl;

	std::ofstream myfile;
	myfile.open(pathToFile);

	// print first line
	myfile << "Duration" << std::endl;
	myfile << timespan.count() << std::endl;

	// saving to the file
	myfile.close();

}
