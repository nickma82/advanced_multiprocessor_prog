/*
* OptimisticSynchronizationSet.cpp
*
* Advanced Multiprocessor Programming
* Author: kpozniak
*
*/

#include <climits>
#include <mutex>
#include "OptimisticSynchronizationSet.h"

bool OptimisticSynchronizationSet::remove(long item) {
	Window w = find(item);
	if (item != w.curr->item) {
		w.unlock();
		return false;
	}
	w.pred->next = w.curr->next;
	w.unlock();
	// Deletion of nodes would be unsafe! // delete(w.curr);
	return true;
}

Window OptimisticSynchronizationSet::find(long item) {
	while (true) {
		// Search for item or successor
		Node* pred = head;
		Node* curr = pred ->next;
		while (curr ->item < item) { pred = curr;
			curr = curr ->next;
		}
		Window w(pred , curr);
		pred->lock();
		curr->lock();
		if (validate(w)) return w;
		w.unlock();
	}
	
}


//checks if a window is valid, this is no item has been deleted or inserted in between
bool OptimisticSynchronizationSet::validate(Window w) { 
	Node* n = head;
	while (n->item <= w.pred->item) {
		if (n == w.pred)
			return n->next == w.curr;
		n = n->next;
	}
	return false;
}