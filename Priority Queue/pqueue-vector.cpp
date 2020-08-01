/*************************************************************
 * File: pqueue-vector.cpp
 *
 * Implementation file for the VectorPriorityQueue
 * class.
 */
 
#include "pqueue-vector.h"
#include "error.h"

VectorPriorityQueue::VectorPriorityQueue() {
	// TODO: Fill this in!
}

VectorPriorityQueue::~VectorPriorityQueue() {
	// TODO: Fill this in!
}

int VectorPriorityQueue::size() {
	return vec.size();
}

bool VectorPriorityQueue::isEmpty() {	
	return size() == 0;
}

void VectorPriorityQueue::enqueue(string value) {
	vec.add(value);
}

string VectorPriorityQueue::peek() {
	if (isEmpty()) {
		error("Queue is empty");
	}
	int index = nextIndex();
	return vec[index];
}

string VectorPriorityQueue::dequeueMin() {
	if (isEmpty()) {
		error("Queue is empty");
	}
	int index = nextIndex();
	string top = vec[index];
	vec.remove(index);
	return top;
}

string VectorPriorityQueue::toString() {
	return vec.toString();
}

int VectorPriorityQueue::nextIndex() {
	string top = vec[0];
	int index = 0;
	for (int i = 1; i < vec.size(); ++i) {
		string str = vec[i];
		if (str <= top) {
			index = i;
			top = str;
		}
	}
	return index;
}
