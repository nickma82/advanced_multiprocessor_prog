/*
* OptimiticSynchronizationSet.h
*
* Advanced Multiprocessor Programming
* Author: kpozniak
*
*/

#ifndef AMP_OPTIMISTIC_SYNCHRONIZATION_SET
#define AMP_OPTIMISTIC_SYNCHRONIZATION_SET

#include "FineGrainedLockingSet.h"

class OptimisticSynchronizationSet: public FineGrainedLockingSet
{
	
protected:
	bool validate (Window<Node> w);
	virtual Window<Node> find(long l);

	
public:
	virtual bool remove(long item);
		
};



#endif  // AMP_OPTIMISTIC_SYNCHRONIZATION_SET