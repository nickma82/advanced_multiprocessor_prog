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
	bool validate (Window w);
	Window find(long l);

	
public:
	bool remove(long item);
		
};



#endif  // AMP_OPTIMISTIC_SYNCHRONIZATION_SET