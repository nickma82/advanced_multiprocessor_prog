/*
 * AMPReferenceSet.h
 *
 * Advanced Multiprocessor Programming
 * Author: kpozniak
 *
 */

#ifndef AMP_REFERENCE_SET
#define AMP_REFERENCE_SET

#include <mutex>
#include <set>
#include "AMPSet.h"


//Reference implementation of AMPSet, only wraps std::set with a global lock
class AMPReferenceSet: public AMPSet
{
	
private:
	std::mutex mutex;
	std::set<long> set;

public:
	virtual bool add(long item);
	
	virtual bool remove(long item);

	virtual bool contains(long item);

};

#endif  // AMP_REFERENCE_SET