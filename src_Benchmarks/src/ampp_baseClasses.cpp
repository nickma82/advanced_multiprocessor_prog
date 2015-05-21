//============================================================================
// Name        : ampp_baseClasses.cpp
// Author      : Nick Ma
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <chrono>
#include <vector>
#include <stdexcept>

#include <thread>
using namespace std;


/* Memory Management */
//class MemoryPool
//class MemoryManager

class TimeMeasurement {
public:
	void time_start() {
		auto tmpTimeNow = std::chrono::high_resolution_clock::now();

		_setMeasurementRunning(true);
		_startTime = tmpTimeNow;
		_invalidateTimeSpan();
	}

	void time_stop() {
		auto tmpTimeNow = std::chrono::high_resolution_clock::now();

		_setMeasurementRunning(false);
		_timespan = std::chrono::duration_cast<std::chrono::milliseconds>
				(tmpTimeNow - _startTime);
	}

	std::chrono::milliseconds getTimeSpan() const {
		if(_timespan == INVALIDTIMESPAN)
			throw std::runtime_error("requested timespan currently invalid");

		return _timespan;
	}

private:
	const std::chrono::milliseconds INVALIDTIMESPAN = std::chrono::seconds(0);

	bool _measurementRunning = false;
	std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
	std::chrono::milliseconds _timespan;

	void _invalidateTimeSpan() {
		_timespan = INVALIDTIMESPAN;
	}

	void _setMeasurementRunning(const bool &nextState) {
		/* consistency check */
		if(_measurementRunning == nextState)
			throw std::runtime_error(
					std::string("cant set the clock twice to the same value: ") +
					std::string(nextState?"1":"0") );

		_measurementRunning = nextState;
	}
};

/* to be tested
- how long Inserting
- how fair is it
 -- e.g. insert: every thread inserts 30sec, how is the count
 -- e.g. insert/remove: every threads inserts one, removes one how often did every thread that
 -- e.g. random add/remove with seed
- throughput
 -- due to fair
- latency
 -- add and or remove
*/
class ConcreteBenchmark : public TimeMeasurement{
public:
	unsigned int counter_insert = 0;
	unsigned int counter_remove = 0;
	unsigned int counter_contains = 0;
};


class BenchmarkAnalyser {
	void sumConcreteBenchmars(std::vector<ConcreteBenchmark>& benchmarks) {}
};



int main() {

	TimeMeasurement measure;
	measure.time_start();
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	measure.time_stop();

	cout << "Time needed:" << measure.getTimeSpan().count() << endl;
	return 0;
}
