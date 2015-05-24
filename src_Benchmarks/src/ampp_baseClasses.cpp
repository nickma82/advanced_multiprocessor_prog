//============================================================================
// Name        : ampp_baseClasses.cpp
// Author      : Nick Ma
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <thread>

#include <memory> //for shared_ptr
#include <map>
#include <list>
#include <utility> //pair, make_pair
#include "TimeMeasurement.h"


class TimerValue {
public:
	DURATION_BASE	duration = std::chrono::seconds(0);
	TimerType 		timerType = ID_UNDEFINED;

	TimerValue(const DURATION_BASE &duration, const TimerType &type = ID_UNDEFINED) :
		duration(duration), timerType(type) {};

	virtual ~TimerValue() {};

	virtual void printValues() {
		std::cout << "time:" << duration.count()
				<< " type:" << timerType << std::endl;
	}
};
/**
 * Every thread gets a TimerStore
 */
class TimerStore {
public:
	/**
	 * List of pairs holding the TimerType and the corresponding value
	 * adding elements to a list has constant complexity
	 */
	std::list<TimerValue> measuredValues;

	void addTimeMeasurement(StartStopTimer &measurement) {
		//auto value = std::pair<TimerType, DURATION_BASE>(timer.type, timer.getTimeSpan());
		measuredValues.push_back( TimerValue(measurement.getTimeSpan(), measurement.type) );
	}

//	std::map<TimerType, std::shared_ptr<StartStopTimer>> timerPool;
//	{	for(TimerType id: allTimerIDs) {
//			auto newTimerObject = std::shared_ptr<StartStopTimer>(new StartStopTimer(id));
//			timerPool.insert( {id, newTimerObject} );
//		}
//		std::cout << "created " << timerPool.size() << " timer objects, they are now ready to use"<< std::endl;
//	}
};


class ThreadTimerValue : public TimerValue {
public:
	unsigned int	threadID = 0;

	ThreadTimerValue(const TimerValue &tv, const unsigned int id) :
		TimerValue(tv), threadID(id) {};

	ThreadTimerValue(const DURATION_BASE &duration, const TimerType &type, const unsigned int id) :
		TimerValue(duration, type), threadID(id) {};

	virtual ~ThreadTimerValue() {};

	virtual void printValues() {
		std::cout << "time[us]:" << duration.count()
				<< " type:" << timerType
				<< " id:" << threadID << std::endl;
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
class TimerAnalyser {
	void addTimesFromTimerStore(const TimerStore &store) {

	}

//	void sumTimerType
};




int main() {
	std::cout << "### TimeMeasure tests" << std::endl;
	StartStopTimer measure;
	measure.time_start();
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	measure.time_stop();
	std::cout << "Time needed:" << measure.getTimeSpan().count() << std::endl;


	std::cout << "### Timer Value tests" << std::endl;
	TimerValue tv(std::chrono::seconds(1), ID_INSERT);
	tv.printValues();
	ThreadTimerValue ttv(tv, 5);
	ttv.printValues();
	ThreadTimerValue ttv2(std::chrono::seconds(1), ID_INSERT, 3);
	ttv2.printValues();

	std::cout << "### TimerStore tests, which every thread uses to store different time measurements" << std::endl;
	TimerStore ts;
	ts.addTimeMeasurement(measure);
	ts.addTimeMeasurement(measure);


	return 0;
}
