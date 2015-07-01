/*
* LazySynchronizationSet.h
*
* Advanced Multiprocessor Programming
* Author: kpozniak
*
*/

#ifndef AMP_LAZY_SYNCHRONIZATION_SET
#define AMP_LAZY_SYNCHRONIZATION_SET

#include <mutex>
#include "AMPSet.h"

class LssNode {
	std::mutex mutex; // critical section lock

public:
	long item;		// value of the node
	LssNode* next;	// pointer to the next node in the list

	// tells if it's logically removed from the set
	//  (s.t. you dont have to lock anything anymore during a contains check)
	bool marked;

	LssNode(long item, LssNode* next);
	void lock();
	void unlock();
};

class LssWindow {
public:
	LssNode* pred;
	LssNode* curr;
	LssWindow(LssNode* pred, LssNode* curr);

	/**
	 * unlocks all necessary nodes, in the right order
	 */
	void unlock();
};

class LazySynchronizationSet: public AMPSet
{
	
	
protected:
	LssNode* head;
	
private:	
	LssWindow find(long l);
	bool validate(LssWindow w);

	
public:
	LazySynchronizationSet();
	~LazySynchronizationSet();
	
	/**
	 * locks
	 */
	bool add(long item);
	
	/**
	 * locks
	 */
	bool remove(long item);
	
	/**
	 * doesn't call find and therefore does not
	 * need to lock
	 */
	bool contains(long item);
		
};



#endif  // AMP_LAZY_SYNCHRONIZATION_SET
