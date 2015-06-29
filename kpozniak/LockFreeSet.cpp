/*
* LockFreeSet.cpp
*
* Advanced Multiprocessor Programming
* Author: kpozniak
*
*/
#include <climits>
#include "LockFreeSet.h"

LockFreeSet::LockFreeSet() {
	head = new LfsNode(LONG_MIN, new LfsNode(LONG_MAX, nullptr));
}

LockFreeSet::~LockFreeSet() {
	LfsNode* node = head;
	while(node != nullptr) {
		LfsNode* next = getPointer(node->next);
		delete node;
		node = next;
	}
}

bool LockFreeSet::add(long item) {
	LfsNode *n = new LfsNode(item, nullptr);
	while (true) {
		LfsWindow w = find(item);
		
		if(isMarked(w.curr)) {
			continue;
		}
		
		if (getPointer(w.curr)->item == item) {
			delete n;
			return false;
		}
		
		n->next = getPointer(w.curr); //unmark
		
		LfsNode* nMarked = n;
		
		if (__sync_bool_compare_and_swap(&(getPointer(w.pred)->next), w.curr, nMarked)) {
			return true;
		}
	}
}

bool LockFreeSet::remove(long item) {
	while (true) {
		LfsWindow w = find(item);
		if (item != getPointer(w.curr)->item) {
			return false;
		}
		
		LfsNode* next = getPointer(w.curr)->next;
		
		if(isMarked(next)){  //next may be marked by another thread!
			return false;
		}
		
		// mark as deleted, verify and set
		LfsNode* markedsucc = mark(next);
		
		if (!__sync_bool_compare_and_swap(&(getPointer(w.curr)->next), next, markedsucc)) {
			continue;
		}
		
		if(isMarked(w.curr)) { //mark next pointer if curr was marked, otherwise a mark could get overridden
		    next = markedsucc;
		}		
		
		// unlink curr
		__sync_bool_compare_and_swap(&(getPointer(w.pred)->next), w.curr, next);
		
		return true;
	}
}

bool LockFreeSet::contains(long item) {
	// same as lazy implementation
	// except marked flag is part of next pointer
	LfsNode* n = head;
	while (n->item < item) n = getPointer(n->next);
	return (n->item == item && !isMarked(n->next));
}

LfsWindow LockFreeSet::find(long item) {
	// Search for item or successor
retry:
	LfsNode* pred_withMark = head;   
	LfsNode* curr_withMark = pred_withMark->next; //dereferencing head is safe, can not be marked
	while (true) {
		LfsNode* succ_withMark = getPointer(curr_withMark)->next;
		while (isMarked(succ_withMark)) {

			LfsNode* succ_withMarkMark = succ_withMark;
			if(!isMarked(curr_withMark)) {  //do not mark an unmarked node
				succ_withMarkMark = getPointer(succ_withMarkMark);
			}
			
			// link out marked item (curr)
			if (!__sync_bool_compare_and_swap(&(getPointer(pred_withMark)->next), curr_withMark, succ_withMarkMark)) {
				goto retry;
			}
			curr_withMark = succ_withMark;
			succ_withMark = getPointer(succ_withMark)->next;
		}
		if (getPointer(curr_withMark)->item >= item) {
			return LfsWindow (pred_withMark, curr_withMark);
		}
		pred_withMark = curr_withMark;
		curr_withMark = getPointer(curr_withMark)->next;
	} 
	
}


LfsWindow::LfsWindow(LfsNode* pred, LfsNode* curr) {
	this->pred = pred;
	this->curr = curr;
}

LfsNode::LfsNode(long item, LfsNode* next){
	this->item = item;
	this->next = next;
}


inline LfsNode* LockFreeSet::getPointer(LfsNode* n) {
	return (LfsNode*)((unsigned long)n bitand 0x7fffffffffffffff);
}

inline LfsNode* LockFreeSet::mark(LfsNode* n) {
	return (LfsNode*)((unsigned long)n bitor 0x8000000000000000);
}

inline bool LockFreeSet::isMarked(LfsNode* n) {
	return ((unsigned long)n) > 0x8000000000000000;
}