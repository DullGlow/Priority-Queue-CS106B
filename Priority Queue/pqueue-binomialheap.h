/**********************************************
 * File: pqueue-heap.h
 *
 * A priority queue class backed by a binary
 * heap.
 */
#ifndef PQueue_BinomialHeap_Included
#define PQueue_BinomialHeap_Included

#include <string>
using namespace std;

/* A class representing a priority queue backed by an
 * binary heap.
 */
class BinomialHeapPriorityQueue {
public:
	/* Constructs a new, empty priority queue backed by a binary heap. */
	BinomialHeapPriorityQueue();
	
	/* Cleans up all memory allocated by this priority queue. */
	~BinomialHeapPriorityQueue();
	
	/* Returns the number of elements in the priority queue. */
	int size();
	
	/* Returns whether or not the priority queue is empty. */
	bool isEmpty();
	
	/* Enqueues a new string into the priority queue. */
	void enqueue(string value);
	
	/* Returns, but does not remove, the lexicographically first string in the
	 * priority queue.
	 */
	string peek();
	
	/* Returns and removes the lexicographically first string in the
	 * priority queue.
	 */
	string dequeueMin();
	
	/* Converts the queue to a printable string representation. */
	string toString();

private:
	struct Node {
		string data;
		Node* parent;
        Node* sibling; // Right sibling
        Node* child; // Left-most child
        int order;

		Node(string value) {		
			data = value;
			parent = NULL;
			child = NULL;
			sibling = NULL;
			order = 0;
		}
	};

	Node* head;

    // Deletes entire heap
    void deleteHeap(Node* heap);

	// Merges two heaps and returns new heap's head node
	Node* mergeHeaps(Node* heap1, Node* heap2);

    // Adds heap to root list
	void add(Node* node);

    // Finds node, which comes before the minimum node
	Node* findMinPrevious();

	// Used for toString()
	string heapToString(Node* heapHead, bool* drawVerticalBarAt);
	string writeLine(Node* node, bool* drawVerticalBarAt);
	string indent(int n, bool* drawVerticalBarAt);
	int findDepth(Node* node);
};

#endif
