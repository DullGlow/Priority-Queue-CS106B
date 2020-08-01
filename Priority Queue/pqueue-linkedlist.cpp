/*************************************************************
 * File: pqueue-linkedlist.cpp
 *
 * Implementation file for the LinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-linkedlist.h"
#include "error.h"


LinkedListPriorityQueue::LinkedListPriorityQueue() {
	head = NULL;
}

LinkedListPriorityQueue::~LinkedListPriorityQueue() {
	node* currentNode = head;
	while (currentNode != NULL) {
        node* nextNode = currentNode->next;
		delete currentNode;
        currentNode = nextNode;
	}
}

int LinkedListPriorityQueue::size() {
	if (isEmpty())
		return 0;
	int counter = 1;
	node* currentNode = head;
    while (currentNode->next != NULL) {
		counter++;
        currentNode = currentNode->next;
	}
	return counter;
}

bool LinkedListPriorityQueue::isEmpty() {
	return head == NULL;
}

void LinkedListPriorityQueue::enqueue(string value) {
	if (head == NULL) {
        head = new node(value);
		return;
	}
	node* lastNode = NULL;
	node* currentNode = head;
    while (true) {
        if (value < currentNode->data) {
			insert(value, lastNode);
			return;
		}
        lastNode = currentNode;
        if (currentNode->next == NULL)
            break;
        currentNode = currentNode->next;
	}
	insert(value, currentNode);
}

string LinkedListPriorityQueue::peek() {
	if (isEmpty()) {
		error("List is empty.");
	}
    return head->data;
}

string LinkedListPriorityQueue::dequeueMin() {
	if (isEmpty()) {
		error("List is empty.");
	}
    string result = head->data;
    node* next = head->next;
	delete head;
	head = next;
	return result;
}

string LinkedListPriorityQueue::toString() {
	string result = "{";
	
	node* currentNode = head;
	while (currentNode != NULL) {
        result += currentNode->data;
        node* nextNode = currentNode->next;
		if (nextNode != NULL)
			result += " -> ";
		currentNode = nextNode;
	}

	result += "}";
	return result;
}

void LinkedListPriorityQueue::insert(string value, node* preceding) {
    node* newNode = new node(value);
	// Inserting before head
	if (preceding == NULL) {
        newNode->next = head;
		head = newNode;
	}
	// Inserting inbetween nodes (or at the end)
	else {
        newNode->next = preceding->next;
        preceding->next = newNode;
	}
}
