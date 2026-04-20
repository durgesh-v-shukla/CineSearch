#pragma once
#include <vector>
#include "Content.h"
using namespace std;

class MaxHeap {
private:
    vector<Content> heap;
    vector<Content> topCache;
    bool topCacheReady = false;

    // This function returns parent index.
    int parent(int i);

    // This function returns left child index.
    int leftChild(int i);

    // This function returns right child index.
    int rightChild(int i);

    // This function moves a node up to maintain heap property.
    void heapifyUp(int i);

    // This function moves a node down to maintain heap property.
    void heapifyDown(int i);

    // This function precomputes descending order cache for fast top-k queries.
    void buildTopCache();

public:
    // This function builds heap from all titles.
    void buildIndex(vector<Content>& titles);

    // This function inserts one content in heap.
    void insertContent(Content c);

    // This function removes and returns max element.
    Content extractTop();

    // This function returns top k records without modifying original heap.
    vector<Content> getTopK(int k);

    // This function returns true when heap is empty.
    bool isEmpty();

    // This function returns heap size.
    int size();
};
