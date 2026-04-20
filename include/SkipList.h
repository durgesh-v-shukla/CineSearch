#pragma once
#include <vector>
#include <random>
#include "Content.h"
using namespace std;

const int MAX_LEVELS = 4;
const float PROB = 0.5f;

struct SkipNode {
    int key;
    Content data;
    SkipNode* forward[MAX_LEVELS];

    SkipNode(int k, Content d) {
        key = k;
        data = d;
        for (int i = 0; i < MAX_LEVELS; i++) {
            forward[i] = nullptr;
        }
    }
};

class SkipList {
private:
    SkipNode* header;
    int currentLevel;
    int totalRecords;
    mt19937 rng;

    // This function decides random level using coin flips.
    int randomLevel();

public:
    // This constructor initializes skip list header.
    SkipList();

    // This destructor frees skip list nodes.
    ~SkipList();

    // This function builds skip list index from all titles.
    void buildIndex(vector<Content>& titles);

    // This function inserts one content by release year key.
    void insert(Content c);

    // This function runs year range query.
    vector<Content> rangeQuery(int yearL, int yearR);

    // This function returns number of records.
    int size();
};
