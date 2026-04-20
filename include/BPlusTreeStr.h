#pragma once
#include <vector>
#include <string>
#include "Content.h"
using namespace std;

const int BP_ORDER_STR = 4;

struct BPlusNodeStr {
    bool isLeaf;
    vector<string> keys;
    vector<Content> records;
    vector<BPlusNodeStr*> children;
    BPlusNodeStr* next;
    BPlusNodeStr* prev;

    BPlusNodeStr(bool leaf) {
        isLeaf = leaf;
        next = nullptr;
        prev = nullptr;
    }
};

class BPlusTreeStr {
private:
    BPlusNodeStr* root;
    int totalRecords;

    // This function finds the leaf where a string key should be present.
    BPlusNodeStr* findLeaf(const string& key);

    // This function inserts one record in a non-full leaf in sorted order.
    void insertInLeaf(BPlusNodeStr* leaf, const Content& c);

    // This function splits a full leaf and returns the new right leaf.
    BPlusNodeStr* splitLeaf(BPlusNodeStr* leaf, string& pushUpKey);

    // This function splits a full internal node and returns new right node.
    BPlusNodeStr* splitInternal(BPlusNodeStr* node, string& pushUpKey);

    // This function recursively inserts and handles split propagation.
    BPlusNodeStr* insertRecursive(BPlusNodeStr* node, const Content& c, string& pushUpKey);

    // This function recursively frees all nodes.
    void clearTree(BPlusNodeStr* node);

public:
    // This constructor initializes an empty B+ tree with leaf root.
    BPlusTreeStr();

    // This destructor frees tree memory.
    ~BPlusTreeStr();

    // This function builds index from full title dataset.
    void buildIndex(vector<Content>& titles);

    // This function inserts a single content record.
    void insert(const Content& c);

    // This function finds exact title match.
    Content* exactSearch(const string& target);

    // This function returns all records with the given prefix.
    vector<Content> prefixSearch(const string& prefix);

    // This function returns total inserted records.
    int size();
};
