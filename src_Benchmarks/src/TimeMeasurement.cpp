#include "TimeMeasurement.h"

#include <stdexcept>

void StartStopTimer::time_start() {
	setMeasurementRunning(true);
	invalidateTimeSpan();

	_startTime = high_resolution_clock::now();
}

void StartStopTimer::time_stop() {
	auto tmpTimeNow = high_resolution_clock::now();

	setMeasurementRunning(false);
	timespan = duration_cast < DURATION_BASE > (tmpTimeNow - _startTime);
}

DURATION_BASE StartStopTimer::getTimeSpan() const {
	if (timespan == INVALID_TIMESPAN)
		throw std::runtime_error("requested timespan currently invalid");

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
