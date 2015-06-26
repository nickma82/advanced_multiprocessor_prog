#ifndef SRC_TIMEMEASUREMENT_H_
#define SRC_TIMEMEASUREMENT_H_

#include <array>
#include <chrono>
using namespace std::chrono;

#define DURATION_BASE		std::chrono::nanoseconds
#define INVALID_TIMESPAN 	std::chrono::seconds(0)

// http://stackoverflow.com/questions/8498300/allow-for-range-based-for-with-enum-classes
enum TimerType { ID_INSERT, ID_REMOVE, ID_CONTAINS, ID_UNDEFINED };
// double-braces required in C++11 (not in C++14) - http://en.cppreference.com/w/cpp/container/array
const std::array<TimerType, 3> allTimerIDs {{ID_INSERT, ID_REMOVE, ID_CONTAINS}};

class StartStopTimer {
public:
	const TimerType type;
	DURATION_BASE timespan = INVALID_TIMESPAN;

	StartStopTimer(const TimerType id = ID_UNDEFINED) : type(id) {};

	/**
	 * Starts the time measurement
	 *
	 * @note as a sideeffect timer_start() invalidates previously
	 * measured timeSpans
	 */
	void time_start();

	/**
	 * Stops the time measurement
	 */
	void time_stop();

	/**
	 * Returns the timespan measured
	 * @note valid after calling time_stop()
	 */
	DURATION_BASE getTimeSpan() const;

	void outputToFile(const std::string &pathToFile);

private:

	bool _measurementRunning = false;
	time_point<high_resolution_clock> _startTime;

	/**
	 * Sets the timespan to invalid
	 */
	void invalidateTimeSpan();

	/**
	 * Checks and sets _measurementRunning for consistency in
	 * order to forbid setting the same state twice
	 *
	 * @var[in] nextState 	state to be set next
	 */
	void setMeasurementRunning(const bool &nextState);
};




#endif /* SRC_TIMEMEASUREMENT_H_ */
