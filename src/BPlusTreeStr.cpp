#include "BPlusTreeStr.h"
using namespace std;

// This constructor initializes an empty B+ tree with leaf root.
BPlusTreeStr::BPlusTreeStr() {
    root = new BPlusNodeStr(true);
    totalRecords = 0;
}

// This destructor frees tree memory.
BPlusTreeStr::~BPlusTreeStr() {
    clearTree(root);
}

// This function recursively frees all nodes.
void BPlusTreeStr::clearTree(BPlusNodeStr* node) {
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

// This function finds the leaf where a string key should be present.
BPlusNodeStr* BPlusTreeStr::findLeaf(const string& key) {
    BPlusNodeStr* node = root;
    while (!node->isLeaf) {
        int i = 0;
        while (i < (int)node->keys.size() && key >= node->keys[i]) {
            i++;
        }
        node = node->children[i];
    }
    return node;
}

// This function inserts one record in a non-full leaf in sorted order.
void BPlusTreeStr::insertInLeaf(BPlusNodeStr* leaf, const Content& c) {
    int i = 0;
    while (i < (int)leaf->keys.size() && leaf->keys[i] < c.title) {
        i++;
    }
    leaf->keys.insert(leaf->keys.begin() + i, c.title);
    leaf->records.insert(leaf->records.begin() + i, c);
}

// This function splits a full leaf and returns the new right leaf.
BPlusNodeStr* BPlusTreeStr::splitLeaf(BPlusNodeStr* leaf, string& pushUpKey) {
    BPlusNodeStr* newLeaf = new BPlusNodeStr(true);
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

// This function splits a full internal node and returns new right node.
BPlusNodeStr* BPlusTreeStr::splitInternal(BPlusNodeStr* node, string& pushUpKey) {
    BPlusNodeStr* newInternal = new BPlusNodeStr(false);
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

// This function recursively inserts and handles split propagation.
BPlusNodeStr* BPlusTreeStr::insertRecursive(BPlusNodeStr* node, const Content& c, string& pushUpKey) {
    if (node->isLeaf) {
        insertInLeaf(node, c);
        if ((int)node->keys.size() > BP_ORDER_STR) {
            return splitLeaf(node, pushUpKey);
        }
        return nullptr;
    }

    int i = 0;
    while (i < (int)node->keys.size() && c.title >= node->keys[i]) {
        i++;
    }

    string childPushUpKey = "";
    BPlusNodeStr* newChild = insertRecursive(node->children[i], c, childPushUpKey);
    if (newChild == nullptr) {
        return nullptr;
    }

    node->keys.insert(node->keys.begin() + i, childPushUpKey);
    node->children.insert(node->children.begin() + i + 1, newChild);

    if ((int)node->keys.size() > BP_ORDER_STR) {
        return splitInternal(node, pushUpKey);
    }
    return nullptr;
}

// This function builds index from full title dataset.
void BPlusTreeStr::buildIndex(vector<Content>& titles) {
    for (int i = 0; i < (int)titles.size(); i++) {
        insert(titles[i]);
    }
}

// This function inserts a single content record.
void BPlusTreeStr::insert(const Content& c) {
    string pushUpKey = "";
    BPlusNodeStr* newNode = insertRecursive(root, c, pushUpKey);

    if (newNode != nullptr) {
        BPlusNodeStr* newRoot = new BPlusNodeStr(false);
        newRoot->keys.push_back(pushUpKey);
        newRoot->children.push_back(root);
        newRoot->children.push_back(newNode);
        root = newRoot;
    }
    totalRecords++;
}

// This function finds exact title match.
Content* BPlusTreeStr::exactSearch(const string& target) {
    BPlusNodeStr* leaf = findLeaf(target);
    for (int i = 0; i < (int)leaf->records.size(); i++) {
        if (leaf->records[i].title == target) {
            return &leaf->records[i];
        }
    }
    return nullptr;
}

// This function returns all records with the given prefix.
vector<Content> BPlusTreeStr::prefixSearch(const string& prefix) {
    vector<Content> results;
    BPlusNodeStr* leaf = findLeaf(prefix);
    string upper = prefix + "~";

    while (leaf != nullptr) {
        for (int i = 0; i < (int)leaf->records.size(); i++) {
            string title = leaf->records[i].title;
            if (title.size() >= prefix.size() && title.compare(0, prefix.size(), prefix) == 0) {
                results.push_back(leaf->records[i]);
            } else if (title > upper) {
                return results;
            }
        }
        leaf = leaf->next;
    }
    return results;
}

// This function returns total inserted records.
int BPlusTreeStr::size() {
    return totalRecords;
}
