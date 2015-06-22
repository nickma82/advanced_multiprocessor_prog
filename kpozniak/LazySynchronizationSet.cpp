/*
* LazySynchronizationSet.cpp
*
* Advanced Multiprocessor Programming
* Author: kpozniak
*
*/

#include <climits>
#include <mutex>
#include "LazySynchronizationSet.h"

LazySynchronizationSet::LazySynchronizationSet() {
	head = new LssNode(LONG_MIN, new LssNode(LONG_MAX, NULL));
}

LazySynchronizationSet::~LazySynchronizationSet() {
	LssNode* node = head;
	while(node != NULL) {
		LssNode* next = node->next;
		delete node;
		node = next;
	}
}

bool LazySynchronizationSet::add(long item) {
	LssWindow w = find(item);
	if (item == w.curr->item) {
		w.unlock();
		return false;
	}
	LssNode* n = new LssNode(item , w.curr);
	w.pred->next = n;
	w.unlock();
	return true;
}

bool LazySynchronizationSet::remove(long item) {
	LssWindow w = find(item);
	
	if (item != w.curr ->item) {
		w.unlock();
		return false;
	}
	w.curr->marked = true;
	// Make sure that marked occurs before unlinking in your memory model!
	w.pred ->next = w.curr ->next;
	w.unlock();
	return true;
}

bool LazySynchronizationSet::contains(long item) {
	LssNode* n = head;
	while (n->item < item) {
		n = n->next;
	}
	return n->item == item && !n->marked;
}

Window<LssNode> LazySynchronizationSet::find(long item) {
	while (true) {
		// Search for item or successor
		LssNode* pred = head;
		LssNode* curr = pred->next;
		while (curr->item < item) {
			pred = curr;
			curr = curr ->next;
		}
		
		Window<LssNode> w(pred, curr);
		pred->lock();
		curr->lock();
		if (validate(w)) return w;
		w.unlock();
	}
	
}

bool LazySynchronizationSet::validate(Window<LssNode> w) {
	return !w.pred->marked &&
	!w.curr->marked &&
	w.pred->next == w.curr;
}

LssNode::LssNode(long item, LssNode* next):Node(item, next){
	this->next = next;
	this->marked = false;
}
