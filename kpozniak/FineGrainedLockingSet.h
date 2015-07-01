/*
* FineGrainedLockingSet.h
*
* Advanced Multiprocessor Programming
* Author: kpozniak
*
*/

#ifndef AMP_FINE_GRAINED_LOCKING_SET
#define AMP_FINE_GRAINED_LOCKING_SET

#include <mutex>
#include "AMPSet.h"

class Node {
std::mutex mutex;	// critical section lock

public:
	long item;		// value of the node
	Node* next;		// pointer to the next node in the list
	Node(long item, Node* next);
	void lock();
	void unlock();
};

class Window {
public:
	Node* pred;
	Node* curr;
	Window(Node* pred, Node* curr);
	void unlock();
};

class FineGrainedLockingSet: public AMPSet
{
	
	
protected:
	Node* head;

	/**
	 * FGL locks already in the find
	 */
	virtual Window find(long l);

	
public:
	FineGrainedLockingSet();
	~FineGrainedLockingSet();
	
	bool add(long item);
	
	virtual bool remove(long item);
	
	/**
	 * calles find(..) and locks necessary elements
	 * with that call.
	 */
	virtual bool contains(long item);
	
};



#endif  // AMP_FINE_GRAINED_LOCKING_SET
