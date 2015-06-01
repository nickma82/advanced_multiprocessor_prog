#include "ValueStore.h"

#include "TimeMeasurement.h"

void ValueStore::addTimeMeasurement(StartStopTimer &measurement) {
	measuredValues.push_back(
			TimerValue(measurement.getTimeSpan(), measurement.type));
}

void ValueStore::addMeasurementWithoutTimer(const TimerType type) {
	measuredValues.push_back(TimerValue(INVALID_TIMESPAN, type));
}

size_t ValueStore::getStoredValueCount() const {
	return measuredValues.size();
}
