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
public:
	long item;
	LssNode* next;
	std::mutex mutex;
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
	
	bool add(long item);
	
	bool remove(long item);
	
	bool contains(long item);
		
};



#endif  // AMP_LAZY_SYNCHRONIZATION_SET