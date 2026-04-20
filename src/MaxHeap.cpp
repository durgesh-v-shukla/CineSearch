#include "MaxHeap.h"
#include <algorithm>
#include <queue>
using namespace std;

// This helper builds descending cache once so repeated top-k queries are faster.
void MaxHeap::buildTopCache() {
    topCache.clear();
    if (heap.empty()) {
        topCacheReady = true;
        return;
    }

    vector<Content> copyHeap = heap;
    make_heap(copyHeap.begin(), copyHeap.end(), [](const Content& a, const Content& b) {
        return a.releaseYear < b.releaseYear;
    });

    topCache.reserve(copyHeap.size());
    while (!copyHeap.empty()) {
        pop_heap(copyHeap.begin(), copyHeap.end(), [](const Content& a, const Content& b) {
            return a.releaseYear < b.releaseYear;
        });
        topCache.push_back(copyHeap.back());
        copyHeap.pop_back();
    }
    topCacheReady = true;
}

// This function returns parent index.
int MaxHeap::parent(int i) {
    return (i - 1) / 2;
}

// This function returns left child index.
int MaxHeap::leftChild(int i) {
    return 2 * i + 1;
}

// This function returns right child index.
int MaxHeap::rightChild(int i) {
    return 2 * i + 2;
}

// This function moves a node up to maintain heap property.
void MaxHeap::heapifyUp(int i) {
    while (i > 0 && heap[parent(i)].releaseYear < heap[i].releaseYear) {
        swap(heap[parent(i)], heap[i]);
        i = parent(i);
    }
}

// This function moves a node down to maintain heap property.
void MaxHeap::heapifyDown(int i) {
    int largest = i;
    int left = leftChild(i);
    int right = rightChild(i);

    if (left < (int)heap.size() && heap[left].releaseYear > heap[largest].releaseYear) {
        largest = left;
    }
    if (right < (int)heap.size() && heap[right].releaseYear > heap[largest].releaseYear) {
        largest = right;
    }

    if (largest != i) {
        swap(heap[i], heap[largest]);
        heapifyDown(largest);
    }
}

// This function builds heap from all titles.
void MaxHeap::buildIndex(vector<Content>& titles) {
    heap.clear();
    topCache.clear();
    topCacheReady = false;
    for (int i = 0; i < (int)titles.size(); i++) {
        insertContent(titles[i]);
    }
}

// This function inserts one content in heap.
void MaxHeap::insertContent(Content c) {
    heap.push_back(c);
    heapifyUp((int)heap.size() - 1);
    topCacheReady = false;
}

// This function removes and returns max element.
Content MaxHeap::extractTop() {
    Content top = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) {
        heapifyDown(0);
    }
    topCacheReady = false;
    return top;
}

// This function returns top k records without modifying original heap.
vector<Content> MaxHeap::getTopK(int k) {
    vector<Content> results;

    if (k <= 0 || heap.empty()) {
        return results;
    }

    if (!topCacheReady) {
        buildTopCache();
    }

    int limit = min(k, (int)topCache.size());
    results.reserve(limit);
    for (int i = 0; i < limit; i++) {
        results.push_back(topCache[i]);
    }

    return results;
}

// This function returns true when heap is empty.
bool MaxHeap::isEmpty() {
    return heap.empty();
}

// This function returns heap size.
int MaxHeap::size() {
    return (int)heap.size();
}
