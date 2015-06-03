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

FineGrainedLockingSet::~FineGrainedLockingSet() {
	Node* node = head;
	while(node != NULL) {
		Node* next = node->next;
		delete node;
		node = next;
	}
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
	delete w.curr; // safe to delete
	return true;
}

bool FineGrainedLockingSet::contains(long item) {
	Window w = find(item);
	bool found = (item == w.curr->item);
	w.unlock();
	return found;
}

Window FineGrainedLockingSet::find(long item) {
	// Search for item or successor
	Node* pred = head;
	pred->lock();     //error in slides
	Node* curr = pred->next;
	curr->lock();
	while (curr->item < item) {
		pred->unlock();
		pred = curr;
		curr = curr->next;
		curr->lock();
	}
	return Window(pred, curr);
	
}

void Node::lock() {
	mutex.lock();
}

void Node::unlock() {
	mutex.unlock();
}

void Window::unlock() {
	pred->unlock();
	curr->unlock();
}

Window::Window(Node* pred, Node* curr) {
	this->pred = pred;
	this->curr = curr;
}

Node::Node(long item, Node* next){
	this->item = item;
	this->next = next;
}
