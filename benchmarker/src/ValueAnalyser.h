#ifndef SRC_VALUEANALYSER_H_
#define SRC_VALUEANALYSER_H_

#include <list>
#include "TimeValues.h"
#include "ValueStore.h"

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
class ValueAnalyser {
public:
	std::list<ThreadTimerValue> _threadedTimerValues;

	/**
	 * Add a ValueStore and use the threadID stored in the ValueStore
	 */
	void addValueStore(const ValueStore &store);

	/**
	 * add a ValueStore with user defined threadID
	 */
	void addValueStore(const ValueStore &store, const unsigned int threadID);

	/**
	 *
	 */
	void outputToFile(const std::string &pathToFile="/tmp/outputFile1");
};


#endif /* SRC_VALUEANALYSER_H_ */
