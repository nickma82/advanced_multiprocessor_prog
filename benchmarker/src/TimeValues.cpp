#include "TimeValues.h"
#include <iostream>

TimerValue::TimerValue(const DURATION_BASE &duration, const TimerType &type) :
	duration(duration), timerType(type) {}

void TimerValue::printValues() {}


void ThreadTimerValue::printValues() {
	std::cout << "time:" << duration.count() << " type:" << timerType
			<< " id:" << threadID << std::endl;
}


std::ostream& operator<<(std::ostream& os, const ThreadTimerValue& obj) {
	// write obj to stream
	os << std::setw(10) << std::setfill(' ') << (unsigned int)obj.threadID
			<< std::setw(10) << std::setfill(' ') << obj.duration.count()
			<< std::setw(10) << std::setfill(' ') << obj.timerType;
	return os;
}
