#pragma once
#include <vector>
#include "Content.h"
using namespace std;

const int BASE_YEAR = 1925;
const int MAX_YEAR = 2024;
const int YEAR_SPAN = MAX_YEAR - BASE_YEAR;

class SegmentTree {
private:
    vector<int> tree;
    vector<int> yearCount;
    int n;

    // This function builds segment tree from yearCount array.
    void build(int node, int start, int end);

    // This function queries count in index range [l, r].
    int query(int node, int start, int end, int l, int r);

public:
    // This constructor initializes tree size metadata.
    SegmentTree();

    // This function builds year frequency index from titles.
    void buildIndex(vector<Content>& titles);

    // This function returns fast count in year range [yearL, yearR].
    int countInRange(int yearL, int yearR);

    // This function returns actual titles in year range for display.
    vector<Content> titlesInRange(vector<Content>& all, int yearL, int yearR);
};
