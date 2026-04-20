#pragma once
#include <string>
#include <vector>
#include <cctype>
#include "Content.h"
using namespace std;

struct HashNode {
    string key;
    Content data;
    HashNode* next;

    HashNode(string k, Content d) {
        key = k;
        data = d;
        next = nullptr;
    }
};

class HashTable {
private:
    HashNode** table;
    int size;
    int totalRecords;

    // This function computes polynomial rolling hash index.
    int hashFunction(const string& key);

    // This function converts string to lowercase.
    string toLower(string s);

public:
    // This constructor initializes hash table buckets.
    HashTable(int sz = 10007);

    // This destructor frees all chain nodes.
    ~HashTable();

    // This function builds index from full dataset.
    void buildIndex(vector<Content>& titles);

    // This function inserts one content into hash chain.
    void insert(const Content& c);

    // This function does case-insensitive exact title search.
    Content* search(const string& target);

    // This function returns total stored records.
    int getTotalRecords();
};
