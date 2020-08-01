/**********************************************
 * File: pqueue-extra.h
 *
 * Your own custom priority queue class, if you
 * want to build one for extra credit.
 *
 * Please replace these file comments with a
 * description of how your priority queue works.
 */
#ifndef PQueue_Extra_Included
#define PQueue_Extra_Included

#include <string>
#include "vector.h"
using namespace std;

/*
 * TODO: Describe this class!
 */
class FibonacciHeapPriorityQueue {
public:
	/* Constructs a new, empty priority queue. */
    FibonacciHeapPriorityQueue();
	
	/* Cleans up all memory allocated by this priority queue. */
    ~FibonacciHeapPriorityQueue();
	
	/* Returns the number of elements in the priority queue. */
	int size();
	
	/* Returns whether or not the priority queue is empty. */
	bool isEmpty();
	
	/* Enqueues a new string into the priority queue. */
    void enqueue(string value, int key);
	
	/* Returns, but does not remove, the lexicographically first string in the
	 * priority queue.
	 */
	string peek();
	
	/* Returns and removes the lexicographically first string in the
	 * priority queue.
	 */
	string dequeueMin();

    /* Decreases the key of all nodes with given value and key */
    void decreaseKey(string value, int key, int newKey);
	
	/* Converts the queue to a printable string representation. */
	string toString();

private:
    struct Node{
        string data;
        int key;
        bool loser;
        Node* parent;
        Node* child;
        Node* right;
        Node* left;
        int order;

        Node(string data_, int key_) {
            data = data_;
            key = key_;
            loser = false;
            parent = NULL;
            child = NULL;
            right = NULL;
            left = NULL;
            order = 0;
        }
    };

    Node* minNode;

    Node* head;

    /* Adds node to root list at the right-most position */
    void addHeap(Node* node);

    /* Removes heap from its family */
    void removeHeap(Node* node);

    /* Finds every node with matching data and key */
    Vector<Node*> findNode(string value, int key);

    /* Finds every node with matching data and key in given heap */
    void findNodeInHeap(string value, int key, Node* heap, Vector<Node*>& result);

    /* Decreases the key of specified node */
    void decreaseKey(Node* node, int newKey);

    /* Removes node from heap and adds it to root list. Called after decreaseKey() */
    void cut(Node* node);

    /* Checks if node is violating any rules. Follow-up function for cut() */
    void cascadeCut(Node* node);

    /* Merges all the heaps of same order in root list. Called after dequeueMin() */
    void cleanup();

    /* Inserts node at given position */
    void insertAfter(Node* nodeToInsert, Node* preceeding);

    /* Merges two heaps and returns new heap's head node */
    Node* merge(Node* node1, Node* node2);

    /* Removes child node from its former position and make parent node its parent */
    void makeChildOf(Node* parent, Node* child);

    /* Iterates through root list and updates minNode. Called after cleanup() */
    void updateMin();

    /* Deletes all the children of this node */
    void deleteHeap(Node* node);

    /* Counts amount of nodes heap has */
    int countHeapNodes(Node* node);

    /* Used for toString() */
    string heapToString(Node* heapHead, bool* drawVerticalBarAt);
    string writeLine(Node* node, bool* drawVerticalBarAt);
    string indent(int n, bool* drawVerticalBarAt);
    int findDepth(Node* node);
};

#endif
