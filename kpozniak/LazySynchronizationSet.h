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
#include "OptimisticSynchronizationSet.h"
#include "FineGrainedLockingSet.h"

class LssNode: public Node
{
public:
	LssNode(long item, LssNode* next);
	LssNode* next;
	bool marked;
};

class LazySynchronizationSet: public OptimisticSynchronizationSet
{
	
protected:
	LssNode* head;
	Window <LssNode> find(long l);
	
private:	
	bool validate(Window<LssNode> w);

	
public:
	LazySynchronizationSet();
	~LazySynchronizationSet();
	
	bool add(long item);
	
	bool remove(long item);
	
	bool contains(long item);
		
};



#endif  // AMP_LAZY_SYNCHRONIZATION_SET