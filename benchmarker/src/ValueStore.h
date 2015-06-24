#ifndef SRC_VALUESTORE_H_
#define SRC_VALUESTORE_H_

#include <list>
#include "TimeValues.h"

/**
 * Every thread gets a TimerStore
 */
class ValueStore {
public:
	/**
	 * List of pairs holding the TimerType and the corresponding value
	 * adding elements to a list has constant complexity
	 */
	std::list<TimerValue> measuredValues;
	const unsigned int threadID = 0;

	/**
	 * optional create a value store with or without threadID
	 */
	ValueStore() : threadID(0) {};
	ValueStore(unsigned int threadID) : threadID(threadID) {};


	/**
	 * Adds a measured time with a timer measuremnt
	 * @pre timer has to be stoped
	 */
	void addTimeMeasurement(StartStopTimer &measurement);

	/**
	 * Method for simple counting
	 */
	void addMeasurementWithoutTimer(const TimerType type);

	size_t getMeasuredValueCount() const;
};


//	std::map<TimerType, std::shared_ptr<StartStopTimer>> timerPool;
//	{	for(TimerType id: allTimerIDs) {
//			auto newTimerObject = std::shared_ptr<StartStopTimer>(new StartStopTimer(id));
//			timerPool.insert( {id, newTimerObject} );
//		}
//		std::cout << "created " << timerPool.size() << " timer objects, they are now ready to use"<< std::endl;
//	}

#endif /* SRC_VALUESTORE_H_ */
