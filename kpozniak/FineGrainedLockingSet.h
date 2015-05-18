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


class FineGrainedLockingSet: public AMPSet
{

	
public:
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
	
private:
	Node* head;
	
public:
	FineGrainedLockingSet();
	FineGrainedLockingSet::Window find(long l);
	
	bool add(long item);
	
	bool remove(long item);
	
	bool contains(long item);
	
};

#endif  // AMP_FINE_GRAINED_LOCKING_SET