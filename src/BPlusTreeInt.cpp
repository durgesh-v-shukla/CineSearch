#include "BPlusTreeInt.h"
using namespace std;

// This constructor initializes tree with one empty leaf root.
BPlusTreeInt::BPlusTreeInt() {
    root = new BPlusNodeInt(true);
    totalRecords = 0;
}

// This destructor frees memory.
BPlusTreeInt::~BPlusTreeInt() {
    clearTree(root);
}

// This function recursively frees nodes.
void BPlusTreeInt::clearTree(BPlusNodeInt* node) {
    if (node == nullptr) {
        return;
    }
    if (!node->isLeaf) {
        for (int i = 0; i < (int)node->children.size(); i++) {
            clearTree(node->children[i]);
        }
    }
    delete node;
}

// This function finds the leaf where int key should be present.
BPlusNodeInt* BPlusTreeInt::findLeaf(int key) {
    BPlusNodeInt* node = root;
    while (!node->isLeaf) {
        int i = 0;
        while (i < (int)node->keys.size() && key >= node->keys[i]) {
            i++;
        }
        node = node->children[i];
    }
    return node;
}

// This function inserts one record in sorted leaf.
void BPlusTreeInt::insertInLeaf(BPlusNodeInt* leaf, const Content& c) {
    int i = 0;
    while (i < (int)leaf->keys.size() && leaf->keys[i] < c.releaseYear) {
        i++;
    }
    leaf->keys.insert(leaf->keys.begin() + i, c.releaseYear);
    leaf->records.insert(leaf->records.begin() + i, c);
}

// This function splits a full leaf and gives key for parent.
BPlusNodeInt* BPlusTreeInt::splitLeaf(BPlusNodeInt* leaf, int& pushUpKey) {
    BPlusNodeInt* newLeaf = new BPlusNodeInt(true);
    int mid = (int)leaf->keys.size() / 2;

    for (int i = mid; i < (int)leaf->keys.size(); i++) {
        newLeaf->keys.push_back(leaf->keys[i]);
        newLeaf->records.push_back(leaf->records[i]);
    }

    leaf->keys.resize(mid);
    leaf->records.resize(mid);

    newLeaf->next = leaf->next;
    if (newLeaf->next != nullptr) {
        newLeaf->next->prev = newLeaf;
    }
    leaf->next = newLeaf;
    newLeaf->prev = leaf;

    pushUpKey = newLeaf->keys[0];
    return newLeaf;
}

// This function splits a full internal node.
BPlusNodeInt* BPlusTreeInt::splitInternal(BPlusNodeInt* node, int& pushUpKey) {
    BPlusNodeInt* newInternal = new BPlusNodeInt(false);
    int mid = (int)node->keys.size() / 2;
    pushUpKey = node->keys[mid];

    for (int i = mid + 1; i < (int)node->keys.size(); i++) {
        newInternal->keys.push_back(node->keys[i]);
    }
    for (int i = mid + 1; i < (int)node->children.size(); i++) {
        newInternal->children.push_back(node->children[i]);
    }

    node->keys.resize(mid);
    node->children.resize(mid + 1);
    return newInternal;
}

// This function recursively inserts record and propagates split.
BPlusNodeInt* BPlusTreeInt::insertRecursive(BPlusNodeInt* node, const Content& c, int& pushUpKey) {
    if (node->isLeaf) {
        insertInLeaf(node, c);
        if ((int)node->keys.size() > BP_ORDER_INT) {
            return splitLeaf(node, pushUpKey);
        }
        return nullptr;
    }

    int i = 0;
    while (i < (int)node->keys.size() && c.releaseYear >= node->keys[i]) {
        i++;
    }

    int childPushUpKey = 0;
    BPlusNodeInt* newChild = insertRecursive(node->children[i], c, childPushUpKey);
    if (newChild == nullptr) {
        return nullptr;
    }

    node->keys.insert(node->keys.begin() + i, childPushUpKey);
    node->children.insert(node->children.begin() + i + 1, newChild);

    if ((int)node->keys.size() > BP_ORDER_INT) {
        return splitInternal(node, pushUpKey);
    }
    return nullptr;
}

// This function builds index from all titles.
void BPlusTreeInt::buildIndex(vector<Content>& titles) {
    for (int i = 0; i < (int)titles.size(); i++) {
        insert(titles[i]);
    }
}

// This function inserts one record.
void BPlusTreeInt::insert(const Content& c) {
    int pushUpKey = 0;
    BPlusNodeInt* newNode = insertRecursive(root, c, pushUpKey);

    if (newNode != nullptr) {
        BPlusNodeInt* newRoot = new BPlusNodeInt(false);
        newRoot->keys.push_back(pushUpKey);
        newRoot->children.push_back(root);
        newRoot->children.push_back(newNode);
        root = newRoot;
    }
    totalRecords++;
}

// This function finds one exact year key match and returns first record.
Content* BPlusTreeInt::exactSearch(int target) {
    BPlusNodeInt* leaf = findLeaf(target);
    for (int i = 0; i < (int)leaf->records.size(); i++) {
        if (leaf->records[i].releaseYear == target) {
            return &leaf->records[i];
        }
    }
    return nullptr;
}

// This function returns all records with year in [yearL, yearR].
vector<Content> BPlusTreeInt::rangeQuery(int yearL, int yearR) {
    vector<Content> results;
    BPlusNodeInt* leaf = findLeaf(yearL);

    while (leaf != nullptr) {
        for (int i = 0; i < (int)leaf->records.size(); i++) {
            int y = leaf->records[i].releaseYear;
            if (y >= yearL && y <= yearR) {
                results.push_back(leaf->records[i]);
            } else if (y > yearR) {
                return results;
            }
        }
        leaf = leaf->next;
    }
    return results;
}

// This function returns count of titles in year range.
int BPlusTreeInt::countInRange(int yearL, int yearR) {
    vector<Content> results = rangeQuery(yearL, yearR);
    return (int)results.size();
}

// This function returns latest top k titles by scanning from right.
vector<Content> BPlusTreeInt::topK(int k) {
    vector<Content> results;
    if (k <= 0) {
        return results;
    }

    BPlusNodeInt* node = root;
    while (!node->isLeaf) {
        node = node->children.back();
    }

    BPlusNodeInt* curr = node;
    while (curr != nullptr && (int)results.size() < k) {
        for (int i = (int)curr->records.size() - 1; i >= 0; i--) {
            results.push_back(curr->records[i]);
            if ((int)results.size() >= k) {
                return results;
            }
        }
        curr = curr->prev;
    }
    return results;
}

// This function returns total inserted records.
int BPlusTreeInt::size() {
    return totalRecords;
}
