/*************************************************************
 * File: pqueue-extra.cpp
 *
 * Implementation file for the FibonacciHeapPriorityQueue class.  You
 * do not need to implement this class, but we're happy to
 * give you extra credit if you do!
 */
 
#include "pqueue-fibonacciheap.h"
#include "error.h"
#include <math.h> // for pow() in size()
#include "hashmap.h" // used in cleanup()

FibonacciHeapPriorityQueue::FibonacciHeapPriorityQueue() {
    minNode = NULL;
    head = new Node("", 0);
}

FibonacciHeapPriorityQueue::~FibonacciHeapPriorityQueue() {
    Node* heap = head->right;
    while (heap != NULL) {
        Node* next = heap->right;
        deleteHeap(heap);
        heap = next;
    }
    delete head;
}

int FibonacciHeapPriorityQueue::size() {
    int result = 0;
    Node* heap = head->right;
    while (heap != NULL) {
        result += countHeapNodes(heap);
        heap = heap->right;
    }
    return result;
}

bool FibonacciHeapPriorityQueue::isEmpty() {
    return head->right == NULL;
}
void FibonacciHeapPriorityQueue::enqueue(string value, int key) {
    Node* newNode = new Node(value, key);
    if (minNode == NULL) {
        minNode = newNode;
    }
    addHeap(newNode);
    if (newNode->key < minNode->key){
        minNode = newNode;
    }
}

string FibonacciHeapPriorityQueue::peek() {
    if (isEmpty())
        error("Queue is empty");
    return minNode->data;
}

string FibonacciHeapPriorityQueue::dequeueMin() {
    if (isEmpty())
        error("Queue is empty");
    string result = minNode->data;
    removeHeap(minNode);
    Node* childNode = minNode->child;
    // Promote minNode's children
    while (childNode != NULL) {
        Node* next = childNode->right;
        removeHeap(childNode);
        addHeap(childNode);
        childNode = next;
    }
    delete minNode;
    // Cleanup, so we don't have heaps of same order
    if (!isEmpty())
        cleanup();
    updateMin();
    return result;
}

void FibonacciHeapPriorityQueue::decreaseKey(string value, int key, int newKey) {
    Vector<Node*> nodes = findNode(value, key);
    if (nodes.isEmpty())
        error("No node found with specified properties");
    foreach (Node* node in nodes) {
        decreaseKey(node, newKey);
        if (node->key < minNode->key)
            minNode = node;
    }
}

void FibonacciHeapPriorityQueue::addHeap(Node *node) {
    Node* lastNode = head;
    while (lastNode->right != NULL)
        lastNode = lastNode->right;
    lastNode->right = node;
    node->left = lastNode;
}

void FibonacciHeapPriorityQueue::removeHeap(Node *node) {
    if (node->left != NULL) {
        node->left->right = node->right;
    }
    if (node->right != NULL) {
        node->right->left = node->left;
    }
    Node* parent = node->parent;
    if (parent != NULL) {
        parent->order -= 1;
        if (parent->child == node) {
            parent->child = node->right;
        }
    }
    node->right = NULL;
    node->left = NULL;
    node->parent = NULL;
}

Vector<FibonacciHeapPriorityQueue::Node*>
FibonacciHeapPriorityQueue::findNode(string value, int key) {
    Vector<Node*> result;
    Node* heap = head->right;
    while (heap != NULL) {
        findNodeInHeap(value, key, heap, result);
        heap = heap->right;
    }
    return result;
}

void FibonacciHeapPriorityQueue::findNodeInHeap(string value, int key,
                                           Node* heap, Vector<Node*>& result) {
    if (heap->key > key) // Dead end
        return;
    if (heap->key == key && heap->data == value)
        result += heap;
    Node* child = heap->child;
    while (child != NULL) {
        findNodeInHeap(value, key, child, result);
        child = child->right;
    }
}

void FibonacciHeapPriorityQueue::decreaseKey(Node* node, int newKey) {
    if (newKey >= node->key)
        error("New key is bigger than the current key");
    node->key = newKey;
    Node* parent = node->parent;
    if (parent != NULL && node->key < parent->key) {
        cut(node);
        cascadeCut(parent);
    }
}

void FibonacciHeapPriorityQueue::cut(Node* node) {
    removeHeap(node);
    addHeap(node);
    node->loser = false;
}

void FibonacciHeapPriorityQueue::cascadeCut(Node *node) {
    Node* parent = node->parent;
    if (parent != NULL) {
        if (!node->loser)
            node->loser = true;
        else {
            cut(node);
            cascadeCut(parent);
        }
    }
}

void FibonacciHeapPriorityQueue::cleanup() {
    // key -> order  || value -> heap of that order
    HashMap<int, Node*> map;
    // Remove all the heads from root list and fill map
    while (!isEmpty()) {
        Node* currentHeap = head->right;
        // While heap of current order is present in map
        while (map.containsKey(currentHeap->order)) {
            // We will take same order heap from map and merge
            // it with currentHeap
            removeHeap(currentHeap);
            Node* secondNode = map[currentHeap->order];
            currentHeap = merge(currentHeap, secondNode);
            map.remove(currentHeap->order - 1);
            insertAfter(currentHeap, head);
        }
        map[currentHeap->order] = currentHeap;
        removeHeap(head->right);
    }
    // Now that we have map with unique heap orders
    // we can fill in root list
    Vector<int> keys = map.keys();
    foreach (int key in keys) {
        addHeap(map[key]);
    }
}

void FibonacciHeapPriorityQueue::insertAfter(Node *nodeToInsert, Node *preceeding) {
    nodeToInsert->left = preceeding;
    nodeToInsert->right = preceeding->right;
    preceeding->right = nodeToInsert;
    if (nodeToInsert->right != NULL) {
        nodeToInsert->right->left = nodeToInsert;
    }
}

FibonacciHeapPriorityQueue::Node*
        FibonacciHeapPriorityQueue::merge(Node* node1, Node* node2) {
    Node* result;
    if (node1->key > node2->key) {
        result = node2;
        makeChildOf(node2, node1);
    }
    else {
        result = node1;
        makeChildOf(node1, node2);
    }
    result->order += 1;
    return result;
}

void FibonacciHeapPriorityQueue::makeChildOf(Node* parent, Node* child) {
    removeHeap(child);

    child->parent = parent;

    if (parent->child != NULL) {
        child->right = parent->child;
        parent->child->left = child;
    }
    parent->child = child;
}

void FibonacciHeapPriorityQueue::deleteHeap(Node* node) {
    if (node == NULL)
        return;
    // Save node's child and delete it
    Node* childNode = node->child;
    delete node;
    // Delete child nodes and their children
    while (childNode != NULL) {
        Node* nextNode = childNode->right;
        deleteHeap(childNode);
        childNode = nextNode;
    }
}

void FibonacciHeapPriorityQueue::updateMin() {
    minNode = head->right;
    Node* currentNode = head->right;
    while (currentNode != NULL) {
        if (minNode->key > currentNode->key){
            minNode = currentNode;
        }
        currentNode = currentNode->right;
    }
}

int FibonacciHeapPriorityQueue::countHeapNodes(Node *node) {
    int result = 1;
    Node* child = node->child;
    while (child != NULL) {
        result += countHeapNodes(child);
        child = child->right;
    }
    return result;
}

/******************* Rest is toString() *******************/

string FibonacciHeapPriorityQueue::toString() {
    string result = "";
    bool* drawVerticalBarAt;
    Node* currentNode = head->right;
    while (currentNode != NULL) {
        drawVerticalBarAt = new bool[currentNode->order + 1];
        for (int i = 1; i < currentNode->order + 1; i++)
            drawVerticalBarAt[i] = false;
        drawVerticalBarAt[0] = currentNode->right != NULL;
        result += heapToString(currentNode, drawVerticalBarAt);
        currentNode = currentNode->right;
        delete[] drawVerticalBarAt;
    }
    return result;
}

string FibonacciHeapPriorityQueue::heapToString(Node* node,
                                                bool* drawVerticalBarAt) {
    string result = writeLine(node, drawVerticalBarAt);
    int height = findDepth(node);
    Node* childNode = (*node).child;
    if (childNode == NULL)
        return result;
    drawVerticalBarAt[height+1] = true;
    while (childNode->right != NULL) {
        result += heapToString(childNode, drawVerticalBarAt);
        childNode = childNode->right;
    }
    drawVerticalBarAt[height+1] = false;
    result += heapToString(childNode, drawVerticalBarAt);

    return result;
}

string FibonacciHeapPriorityQueue::writeLine(Node* node, bool* drawVerticalBarAt) {
    string result= "";
    int height = findDepth(node);
    result += indent(height, drawVerticalBarAt);
    if (drawVerticalBarAt[height])
        result += "|--- ";
    else
        result += "`--- ";
    result += node->data;
    result += "(" + integerToString(node->key) + ")";
    if (node->loser)
        result += "(X)";
    result += "\n";
    return result;
}

string FibonacciHeapPriorityQueue::indent(int n, bool* drawVerticalBarAt) {
    string result = "";
    for (int i = 0; i < n; i++) {
        if (drawVerticalBarAt[i])
            result += "|    ";
        else
            result += "     ";
    }
    return result;
}

int FibonacciHeapPriorityQueue::findDepth(Node* node) {
    Node* parent = (*node).parent;
    int counter = 0;
    while (parent != NULL) {
        counter++;
        parent = (*parent).parent;
    }
    return counter;
}


