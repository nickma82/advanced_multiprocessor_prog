#ifndef SRC_TIMEVALUES_H_
#define SRC_TIMEVALUES_H_

#include <chrono>
#include <iostream>
#include <fstream> //file stream
#include <iomanip>
#include "TimeMeasurement.h"

class TimerValue {
public:
	DURATION_BASE	duration = INVALID_TIMESPAN;
	TimerType 		timerType = ID_UNDEFINED;

	TimerValue(const DURATION_BASE &duration, const TimerType &type = ID_UNDEFINED);

	virtual ~TimerValue() {};

	virtual void printValues();
};


class ThreadTimerValue : public TimerValue {
public:
	const unsigned int	threadID = 0;

	/**
	 * Constructor from an existing TimerValue object
	 */
	ThreadTimerValue(const TimerValue &tv, const unsigned int threadID) :
			TimerValue(tv), threadID(threadID) {};

	ThreadTimerValue(const DURATION_BASE&duration, const TimerType &type, const unsigned int threadID) :
			TimerValue(duration, type), threadID(threadID) {};

	virtual ~ThreadTimerValue() {};

	void printValues();
private:
	/* no standard constructor available*/
	ThreadTimerValue();
};

extern std::ostream& operator<<(std::ostream& os, const ThreadTimerValue& obj);


#endif /* SRC_TIMEVALUES_H_ */
