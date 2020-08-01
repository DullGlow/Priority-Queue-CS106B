/*************************************************************
 * File: pqueue-doublylinkedlist.cpp
 *
 * Implementation file for the DoublyLinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-doublylinkedlist.h"
#include "error.h"

DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
	head = NULL;
}

DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
	node* currentNode = head;
	while (currentNode != NULL) {
        node* nextNode = currentNode->next;
		delete currentNode;
		currentNode = nextNode;
	}
}

int DoublyLinkedListPriorityQueue::size() {
	node* currentNode = head;
	int counter = 0;
	while (currentNode != NULL) {
		counter++;
        currentNode = currentNode->next;
	}
	return counter;
}

bool DoublyLinkedListPriorityQueue::isEmpty() {	
	return head == NULL;
}

void DoublyLinkedListPriorityQueue::enqueue(string value) {
	if (head == NULL) {
        head = new node(value);
	}
	else {
        node* newNode = new node(value);
        newNode->next = head;
        head->previous = newNode;
		head = newNode;
	}
}

string DoublyLinkedListPriorityQueue::peek() {
	if (isEmpty())
		error("List is empty.");
	node* result = findMin();
    return result->data;
}

string DoublyLinkedListPriorityQueue::dequeueMin() {
	if (isEmpty())
		error("List is empty.");
	node* min = findMin();
    if (min->next != NULL)
        min->next->previous = min->previous;
	if (min == head) {
        head = min->next;
	}
	else {
        min->previous->next = min->next;
	}
    string result = min->data;
	delete min;
	return result;
}

string DoublyLinkedListPriorityQueue::toString() {
	string result = "";
	node* currentNode = head;
	while (currentNode != NULL) {
        node* next = currentNode->next;
        node* previous = currentNode->previous;
		if (previous != NULL)
			result += " <- ";
        result += currentNode->data;
		if (next != NULL)
			result += " -> ";
		currentNode = next;
	}
	return "{" + result + "}";
}

DoublyLinkedListPriorityQueue::node *DoublyLinkedListPriorityQueue::findMin() {
	node* result = head;
    node* currentNode = head->next;
	while (currentNode != NULL) {
        if (currentNode->data < result->data)
			result = currentNode;
        currentNode = currentNode->next;
	}
	return result;
}
