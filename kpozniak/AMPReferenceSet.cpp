/*
 * AMPReferenceSet.h
 *
 * Advanced Multiprocessor Programming
 * Author: kpozniak
 *
 */

#include <mutex>
#include <set>
#include "AMPReferenceSet.h"


	bool AMPReferenceSet::add(long item) {
		mutex.lock();
		auto result = set.insert(item);
		mutex.unlock();
		return result.second;
	}
	
	bool AMPReferenceSet::remove(long item) {
		mutex.lock();
		auto result = set.erase(item);
		mutex.unlock();
		return result > 0;
	}

	bool AMPReferenceSet::contains(long item) {
		mutex.lock();
		auto result = set.find(item);
		mutex.unlock();
		return result != set.end();
	}
