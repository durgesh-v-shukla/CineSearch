#include "HashTable.h"
using namespace std;

// This constructor initializes hash table buckets.
HashTable::HashTable(int sz) {
    size = sz;
    table = new HashNode*[size];
    for (int i = 0; i < size; i++) {
        table[i] = nullptr;
    }
    totalRecords = 0;
}

// This destructor frees all chain nodes.
HashTable::~HashTable() {
    for (int i = 0; i < size; i++) {
        HashNode* curr = table[i];
        while (curr != nullptr) {
            HashNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
    delete[] table;
}

// This function computes polynomial rolling hash index.
int HashTable::hashFunction(const string& key) {
    long long hash = 0;
    for (int i = 0; i < (int)key.size(); i++) {
        hash = (hash * 31 + tolower((unsigned char)key[i])) % size;
    }
    if (hash < 0) {
        hash = -hash;
    }
    return (int)hash;
}

// This function converts string to lowercase.
string HashTable::toLower(string s) {
    for (int i = 0; i < (int)s.size(); i++) {
        s[i] = (char)tolower((unsigned char)s[i]);
    }
    return s;
}

// This function builds index from full dataset.
void HashTable::buildIndex(vector<Content>& titles) {
    for (int i = 0; i < (int)titles.size(); i++) {
        insert(titles[i]);
    }
}

// This function inserts one content into hash chain.
void HashTable::insert(const Content& c) {
    string key = toLower(c.title);
    int idx = hashFunction(key);
    HashNode* node = new HashNode(key, c);
    node->next = table[idx];
    table[idx] = node;
    totalRecords++;
}

// This function does case-insensitive exact title search.
Content* HashTable::search(const string& target) {
    string key = toLower(target);
    int idx = hashFunction(key);
    HashNode* curr = table[idx];
    while (curr != nullptr) {
        if (curr->key == key) {
            return &curr->data;
        }
        curr = curr->next;
    }
    return nullptr;
}

// This function returns total stored records.
int HashTable::getTotalRecords() {
    return totalRecords;
}
