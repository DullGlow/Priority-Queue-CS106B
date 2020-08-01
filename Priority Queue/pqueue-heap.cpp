/*************************************************************
 * File: pqueue-heap.cpp
 *
 * Implementation file for the HeapPriorityQueue
 * class.
 */
 
#include "pqueue-heap.h"
#include "error.h"

const int STARTING_SIZE = 32;
const int GROW_MULTIPLIER = 2;

HeapPriorityQueue::HeapPriorityQueue() {
	arr = new string[STARTING_SIZE];
	arr[0] = "foo"; // Dummy element
	arrSize = 1;
	allocSize = STARTING_SIZE;
}

HeapPriorityQueue::~HeapPriorityQueue() {
	delete[] arr;
}

int HeapPriorityQueue::size() {
	return arrSize - 1;
}

bool HeapPriorityQueue::isEmpty() {
	return arrSize == 1;
}

void HeapPriorityQueue::enqueue(string value) {
	arr[arrSize++] = value;
	if (arrSize == allocSize)
		grow();
	bubbleUp(arrSize - 1);
}

string HeapPriorityQueue::peek() {
	if (isEmpty())
		error("List is empty.");
	return arr[1];
}

string HeapPriorityQueue::dequeueMin() {
	if (isEmpty())
		error("List is empty.");
	string result = arr[1];
	swap(--arrSize, 1);
	bubbleDown(1);
	return result;
}

void HeapPriorityQueue::copy(string newArr[], int n) {
	for (int i = 0; i < n; i++) {
		newArr[i] = arr[i];
	}
}

void HeapPriorityQueue::swap(int index1, int index2) {
	string temp = arr[index1];
	arr[index1] = arr[index2];
	arr[index2] = temp;
}

void HeapPriorityQueue::grow() {
	allocSize *= GROW_MULTIPLIER;
	string* temp = new string[allocSize];
	copy(temp, arrSize);
	delete[] arr;
	arr = temp;
}

void HeapPriorityQueue::bubbleUp(int index) {
	string current = arr[index];
	string parent = arr[index/2];
	while (current < parent && index > 1) {
		swap(index, index/2);
		index /= 2;
		parent = arr[index/2];
	}
}

void HeapPriorityQueue::bubbleDown(int index) {
	// Two child
	if (index * 2 + 1 < arrSize) {
		int newIndex = index*2 + 1;
		if (arr[index*2] < arr[index*2 + 1])
			newIndex = index*2;
		if (arr[index] > arr[newIndex]) {
			swap(index, newIndex);
			bubbleDown(newIndex);
		}
	}
	// One child
	else if (index * 2 < arrSize) {
		if (arr[index] > arr[index*2])
			swap(index, index*2);
	}
}


/* Rest is toString() */

string HeapPriorityQueue::toString() {
	bool* drawVerticalBarAt = new bool[arrSize];
	drawVerticalBarAt[1] = true;
	string result = createTree(1, drawVerticalBarAt);
	delete[] drawVerticalBarAt;
	return result;
}

string HeapPriorityQueue::createTree(int index, bool* drawVerticalBarAt) {
	string result = writeLine(index, drawVerticalBarAt);
	if (2*index < arrSize) {
		drawVerticalBarAt[rowOf(index) + 1] = (2*index + 1 < arrSize);
		result += createTree(2*index, drawVerticalBarAt);
		drawVerticalBarAt[rowOf(index) + 1] = false;
	}
	if (2*index + 1 < arrSize) {
		result += createTree(2*index + 1, drawVerticalBarAt);
	}
	return result;
}

string HeapPriorityQueue::writeLine(int i, bool* drawVerticalBarAt) {
	if (i == 1)
		return arr[1] + "\n";
	string result= "";
	int row = rowOf(i);
    result += indent(row - 1, drawVerticalBarAt);
	if (drawVerticalBarAt[row])
		result += "|--- ";
	else
		result += "`--- ";
	result += arr[i];
	result += "\n";
	return result;
}

/* To which row does this index belong to
 *
 *	/============================\
 *  | row 0           [A]        |
 *  |               /     \      |
 *  | row 1       [B]     [C]    |
 *  |            /   \   /   \   |
 *  | row 2    [D]  [E] [F]  [G] |
 *	\============================/
*/
int HeapPriorityQueue::rowOf(int index) {
	int row = 0;
	while (index > 1) {
		row++;
		index /= 2;
	}
	return row;
}

string HeapPriorityQueue::indent(int n, bool* drawVerticalBarAt) {
	string result = "";
	for (int i = 1; i <= n; i++) {
		if (drawVerticalBarAt[i])
			result += "|    ";
		else
			result += "     ";
	}
	return result;
}

