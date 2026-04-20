#include "SkipList.h"
using namespace std;

// This constructor initializes skip list header.
SkipList::SkipList() {
    Content dummy;
    dummy.releaseYear = -1;
    header = new SkipNode(-1, dummy);
    currentLevel = 0;
    totalRecords = 0;
    rng.seed(42);
}

// This destructor frees skip list nodes.
SkipList::~SkipList() {
    SkipNode* curr = header;
    while (curr != nullptr) {
        SkipNode* temp = curr;
        curr = curr->forward[0];
        delete temp;
    }
}

// This function decides random level using coin flips.
int SkipList::randomLevel() {
    int level = 0;
    while (level < MAX_LEVELS - 1 && ((float)rng() / (float)rng.max()) < PROB) {
        level++;
    }
    return level;
}

// This function builds skip list index from all titles.
void SkipList::buildIndex(vector<Content>& titles) {
    for (int i = 0; i < (int)titles.size(); i++) {
        insert(titles[i]);
    }
}

// This function inserts one content by release year key.
void SkipList::insert(Content c) {
    SkipNode* update[MAX_LEVELS];
    SkipNode* curr = header;

    for (int i = currentLevel; i >= 0; i--) {
        while (curr->forward[i] != nullptr && curr->forward[i]->key < c.releaseYear) {
            curr = curr->forward[i];
        }
        update[i] = curr;
    }

    int newLevel = randomLevel();
    if (newLevel > currentLevel) {
        for (int i = currentLevel + 1; i <= newLevel; i++) {
            update[i] = header;
        }
        currentLevel = newLevel;
    }

    SkipNode* node = new SkipNode(c.releaseYear, c);
    for (int i = 0; i <= newLevel; i++) {
        node->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = node;
    }

    totalRecords++;
}

// This function runs year range query.
vector<Content> SkipList::rangeQuery(int yearL, int yearR) {
    vector<Content> results;
    SkipNode* curr = header;

    for (int i = currentLevel; i >= 0; i--) {
        while (curr->forward[i] != nullptr && curr->forward[i]->key < yearL) {
            curr = curr->forward[i];
        }
    }

    curr = curr->forward[0];
    while (curr != nullptr && curr->key <= yearR) {
        results.push_back(curr->data);
        curr = curr->forward[0];
    }
    return results;
}

// This function returns number of records.
int SkipList::size() {
    return totalRecords;
}
