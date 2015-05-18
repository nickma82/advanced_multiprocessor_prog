/*
* FineGrainedLockingSet.cpp
*
* Advanced Multiprocessor Programming
* Author: kpozniak
*
*/

#include <climits>
#include <mutex>
#include "FineGrainedLockingSet.h"

FineGrainedLockingSet::FineGrainedLockingSet() {
head = new Node(LONG_MIN, new Node(LONG_MAX, NULL));

}

bool FineGrainedLockingSet::add(long item) {
	Window w = find(item);
	if (item == w.curr->item) {
		w.unlock();
		return false;
	}
	Node* n = new Node(item , w.curr);
	w.pred->next = n;
	w.unlock();
	return true;
}

bool FineGrainedLockingSet::remove(long item) {
	Window w = find(item);
	if (item != w.curr ->item) {
		w.unlock();
		return false;
	}
	w.pred->next = w.curr->next;
	w.unlock();
	//delete(w.curr); // safe to delete
	return true;
}

bool FineGrainedLockingSet::contains(long item) {
	Window w = find(item);
	bool found = (item == w.curr->item);
	w.unlock();
	return found;
}

FineGrainedLockingSet::Window FineGrainedLockingSet::find(long item) {
	// Search for item or successor
	Node* pred = head;
	Node* curr = pred->next;
	// Exception safety for locks ignored for simplicity
	pred->lock();
	curr->lock();
	while (curr->item < item) {
		pred->unlock();
		pred = curr;
		curr = curr->next;
		curr->lock();
	}
	return Window(pred, curr);
	
}

void FineGrainedLockingSet::Node::lock() {
	mutex.lock();
}

void FineGrainedLockingSet::Node::unlock() {
	mutex.unlock();
}

void FineGrainedLockingSet::Window::unlock() {
	pred->unlock();
	curr->unlock();
}

FineGrainedLockingSet::Window::Window(Node* pred, Node* curr) {
	this->pred = pred;
	this->curr = curr;
}

FineGrainedLockingSet::Node::Node(long item, Node* next){
	this->item = item;
	this->next = next;
}
