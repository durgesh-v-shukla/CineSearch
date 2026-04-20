#pragma once
#include <vector>
#include "Content.h"
using namespace std;

const int BP_ORDER_INT = 4;

struct BPlusNodeInt {
    bool isLeaf;
    vector<int> keys;
    vector<Content> records;
    vector<BPlusNodeInt*> children;
    BPlusNodeInt* next;
    BPlusNodeInt* prev;

    BPlusNodeInt(bool leaf) {
        isLeaf = leaf;
        next = nullptr;
        prev = nullptr;
    }
};

class BPlusTreeInt {
private:
    BPlusNodeInt* root;
    int totalRecords;

    // This function finds the leaf where int key should be present.
    BPlusNodeInt* findLeaf(int key);

    // This function inserts one record in sorted leaf.
    void insertInLeaf(BPlusNodeInt* leaf, const Content& c);

    // This function splits a full leaf and gives key for parent.
    BPlusNodeInt* splitLeaf(BPlusNodeInt* leaf, int& pushUpKey);

    // This function splits a full internal node.
    BPlusNodeInt* splitInternal(BPlusNodeInt* node, int& pushUpKey);

    // This function recursively inserts record and propagates split.
    BPlusNodeInt* insertRecursive(BPlusNodeInt* node, const Content& c, int& pushUpKey);

    // This function recursively frees nodes.
    void clearTree(BPlusNodeInt* node);

public:
    // This constructor initializes tree with one empty leaf root.
    BPlusTreeInt();

    // This destructor frees memory.
    ~BPlusTreeInt();

    // This function builds index from all titles.
    void buildIndex(vector<Content>& titles);

    // This function inserts one record.
    void insert(const Content& c);

    // This function finds one exact year key match and returns first record.
    Content* exactSearch(int target);

    // This function returns all records with year in [yearL, yearR].
    vector<Content> rangeQuery(int yearL, int yearR);

    // This function returns count of titles in year range.
    int countInRange(int yearL, int yearR);

    // This function returns latest top k titles by scanning from right.
    vector<Content> topK(int k);

    // This function returns total inserted records.
    int size();
};
