#include "SegmentTree.h"
using namespace std;

// This constructor initializes tree size metadata.
SegmentTree::SegmentTree() {
    n = YEAR_SPAN;
}

// This function builds segment tree from yearCount array.
void SegmentTree::build(int node, int start, int end) {
    if (start == end) {
        tree[node] = yearCount[start];
        return;
    }

    int mid = (start + end) / 2;
    build(2 * node, start, mid);
    build(2 * node + 1, mid + 1, end);
    tree[node] = tree[2 * node] + tree[2 * node + 1];
}

// This function queries count in index range [l, r].
int SegmentTree::query(int node, int start, int end, int l, int r) {
    if (r < start || end < l) {
        return 0;
    }
    if (l <= start && end <= r) {
        return tree[node];
    }

    int mid = (start + end) / 2;
    return query(2 * node, start, mid, l, r) + query(2 * node + 1, mid + 1, end, l, r);
}

// This function builds year frequency index from titles.
void SegmentTree::buildIndex(vector<Content>& titles) {
    yearCount.clear();
    yearCount.resize(n, 0);

    for (int i = 0; i < (int)titles.size(); i++) {
        int idx = titles[i].releaseYear - BASE_YEAR;
        if (idx >= 0 && idx < n) {
            yearCount[idx]++;
        }
    }

    tree.clear();
    tree.resize(4 * n, 0);
    build(1, 0, n - 1);
}

// This function returns fast count in year range [yearL, yearR].
int SegmentTree::countInRange(int yearL, int yearR) {
    int l = yearL - BASE_YEAR;
    int r = yearR - BASE_YEAR;

    if (l < 0) {
        l = 0;
    }
    if (r >= n) {
        r = n - 1;
    }
    if (l > r) {
        return 0;
    }

    return query(1, 0, n - 1, l, r);
}

// This function returns actual titles in year range for display.
vector<Content> SegmentTree::titlesInRange(vector<Content>& all, int yearL, int yearR) {
    vector<Content> results;
    for (int i = 0; i < (int)all.size(); i++) {
        if (all[i].releaseYear >= yearL && all[i].releaseYear <= yearR) {
            results.push_back(all[i]);
        }
    }
    return results;
}
