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
	LfsNode* getPointer(LfsNode* n);
	LfsNode* mark(LfsNode* n);
	bool isMarked(LfsNode* n);
	
public:
	LockFreeSet();
	~LockFreeSet();
	
	bool add(long item);
	
	bool remove(long item);
	
	bool contains(long item);
};



#endif  // AMP_LOCK_FREE_SET