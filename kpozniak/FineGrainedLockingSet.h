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
public:
	long item;
	Node* next;
	std::mutex mutex;
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
	virtual Window find(long l);

	
public:
	FineGrainedLockingSet();
	~FineGrainedLockingSet();
	
	bool add(long item);
	
	virtual bool remove(long item);
	
	virtual bool contains(long item);
	
};



#endif  // AMP_FINE_GRAINED_LOCKING_SET