/*
* LazySynchronizationSet.h
*
* Advanced Multiprocessor Programming
* Author: kpozniak
*
*/

#ifndef AMP_LOCK_FREE_SET
#define AMP_LOCK_FREE_SET

#include <atomic>
#include <mutex>
#include "AMPSet.h"


class LfsNode {
public:
	long item;
	LfsNode* next;
	LfsNode(long item, LfsNode* next);
	bool isMarked();
	void mark();
	void unmark();
	LfsNode* getNext();
};

class LfsWindow {
public:
	LfsNode* pred;
	LfsNode* curr;
	LfsWindow(LfsNode* pred, LfsNode* curr);
};

class LockFreeSet: public AMPSet
{
	
protected:
	LfsNode* head;
	
private:	
	LfsWindow find(long l);
	
public:
	LockFreeSet();
	~LockFreeSet();
	
	bool add(long item);
	
	bool remove(long item);
	
	bool contains(long item);
	
	LfsNode* getPointer(LfsNode* n);
	LfsNode* mark(LfsNode* n);
	LfsNode* unmark(LfsNode* n);
	bool isMarked(LfsNode* n);
};



#endif  // AMP_LOCK_FREE_SET