/*************************************************************
 * File: pqueue-heap.cpp
 *
 * Implementation file for the BinomialHeapPriorityQueue
 * class.
 */
 
#include "pqueue-binomialheap.h"
#include <string>
#include <math.h> // for pow() in size()
#include "strlib.h" // for IntegerToString() in toString()
#include "error.h"

BinomialHeapPriorityQueue::BinomialHeapPriorityQueue() {
	head = new Node("");
}

BinomialHeapPriorityQueue::~BinomialHeapPriorityQueue() {
    Node* heap = head->sibling;
    while (heap != NULL) {
        Node* next = heap->sibling;
        deleteHeap(heap);
        heap = next;
    }
    delete head;
}

int BinomialHeapPriorityQueue::size() {
    int result = 0;
    Node* currentNode = head->sibling;
    // Number of nodes in heap = 2 ^ heap order
    while (currentNode != NULL) {
        result += pow(2, currentNode->order);
        currentNode = currentNode->sibling;
    }
    return result;
}

bool BinomialHeapPriorityQueue::isEmpty() {
    return head->sibling == NULL;
}

void BinomialHeapPriorityQueue::enqueue(string value) {
	Node* newNode = new Node(value);
	add(newNode);
}

string BinomialHeapPriorityQueue::peek() {
	if (isEmpty()) {
		error("Heap is empty.");
	}
    Node* result = findMinPrevious()->sibling;
    return result->data;
}

string BinomialHeapPriorityQueue::dequeueMin() {
	if (isEmpty()) {
		error("Heap is empty.");
	}
	// Find minimum node
	Node* minPreviousNode = findMinPrevious();
    Node* minNode = minPreviousNode->sibling;
    string result = minNode->data; // Save result
    // Remove node from root list
    minPreviousNode->sibling = minNode->sibling;
    Node* childNode = minNode->child;
    delete minNode; // Free up space
    // Add minNode's children to root list
	while (childNode != NULL) {
        childNode->parent = NULL;
        Node* nextNode = childNode->sibling;
		add(childNode);
		childNode = nextNode;
	}
	return result;
}

void BinomialHeapPriorityQueue::deleteHeap(Node* heap) {
    if (heap == NULL)
        return;
    // Save node's child and delete it
    Node* childNode = heap->child;
    delete heap;
    // Delete child nodes and their children
    while (childNode != NULL) {
        Node* nextNode = childNode->sibling;
        deleteHeap(childNode);
        childNode = nextNode;
    }
}

BinomialHeapPriorityQueue::Node*
        BinomialHeapPriorityQueue::mergeHeaps(Node* heap1, Node* heap2) {
    Node* result;
    if (heap1->data >= heap2->data) {
        result = heap2;
        heap2->parent = NULL;
        heap1->parent = heap2;
        heap1->sibling = heap2->child;
        heap2->child = heap1;
    }
    else {
        result = heap1;
        heap1->parent = NULL;
        heap2->parent = heap1;
        heap2->sibling = heap1->child;
        heap1->child = heap2;
    }
    result->order += 1;
    return result;
}

void BinomialHeapPriorityQueue::add(Node* newHeap) {
	Node* previousHeap = head;
    Node* nextHeap = head->sibling;
	// Add heap at the start of binominal heap
    head->sibling = newHeap;
    newHeap->sibling = nextHeap;
    // Move new heap along root list and if:
    // (next heap's order == newHeap's order) -> merge two heaps
    // (next heap's order <  newHeap's order) -> swap positions of the heaps
    // (next heap's order >  newHeap's order) -> stop
    while (nextHeap != NULL && nextHeap->order <= newHeap->order) {
        // Upon reaching a heap of the same order, remove that heap, merge
		// it with newHeap and add the merged heap back into binominal heap
        if (nextHeap->order == newHeap->order) {
			// Save next heap
            Node* next = nextHeap->sibling;
			// Merge two heaps
			Node* tmp = mergeHeaps(nextHeap, newHeap);
            tmp->sibling = next;
            previousHeap->sibling = tmp;
            newHeap = tmp;
            nextHeap = previousHeap;
		}
		// Otherwise just swap heaps
		else {
            newHeap->sibling = nextHeap->sibling;
            previousHeap->sibling = nextHeap;
            nextHeap->sibling = newHeap;
		}
		previousHeap = nextHeap;
        nextHeap = newHeap->sibling;
	}
}

BinomialHeapPriorityQueue::Node* BinomialHeapPriorityQueue::findMinPrevious() {
	Node* previousNode = head;
    Node* currentNode = head->sibling;
	Node* result = head;
    string min = currentNode->data;
	while (currentNode != NULL) {
        string val = currentNode->data;
		if (val < min) {
			min = val;
			result = previousNode;
		}
		previousNode = currentNode;
        currentNode = currentNode->sibling;
	}
	return result;
}

/******************* Rest is toString() ********************/

string BinomialHeapPriorityQueue::toString() {	
	string result = "";
    Node* currentNode = head->sibling;
	bool* drawVerticalBarAt;
	while (currentNode != NULL) {
        drawVerticalBarAt = new bool[currentNode->order + 1];
        for (int i = 1; i < currentNode->order + 1; i++) {
			drawVerticalBarAt[i] = false;
		}
        drawVerticalBarAt[0] = currentNode->sibling != NULL;
		result += heapToString(currentNode, drawVerticalBarAt);
        currentNode = currentNode->sibling;
		delete[] drawVerticalBarAt;
	}
	return result;
}

string BinomialHeapPriorityQueue::heapToString(Node* node, bool* drawVerticalBarAt) {
	string result = writeLine(node, drawVerticalBarAt);
	int height = findDepth(node);
    Node* childNode = node->child;
	if (childNode == NULL)
		return result;
	drawVerticalBarAt[height+1] = true;
    while (childNode->sibling != NULL) {
		result += heapToString(childNode, drawVerticalBarAt);
        childNode = childNode->sibling;
	}
	drawVerticalBarAt[height+1] = false;
	result += heapToString(childNode, drawVerticalBarAt);

	return result;
}

string BinomialHeapPriorityQueue::writeLine(Node* node, bool* drawVerticalBarAt) {
	string result= "";
	int height = findDepth(node);
    result += indent(height, drawVerticalBarAt);
    if (drawVerticalBarAt[height])
        result += "|--- ";
    else
        result += "`--- ";
    result += node->data;
	result += "\n";
	return result;
}

string BinomialHeapPriorityQueue::indent(int n, bool* drawVerticalBarAt) {
	string result = "";
    for (int i = 0; i < n; i++) {
		if (drawVerticalBarAt[i])
			result += "|    ";
		else
			result += "     ";
	}
	return result;
}

int BinomialHeapPriorityQueue::findDepth(Node* node) {
    Node* parent = node->parent;
    int counter = 0;
	while (parent != NULL) {
		counter++;
        parent = parent->parent;
	}
	return counter;
}
