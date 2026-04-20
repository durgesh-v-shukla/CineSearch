#pragma once
#include <vector>
#include <algorithm>
#include "Content.h"
using namespace std;

class LinearSearch {
public:
    // This function does exact title search using full linear scan.
    Content* exactSearch(vector<Content>& titles, string target);

    // This function returns all titles starting with the given prefix.
    vector<Content> prefixSearch(vector<Content>& titles, string prefix);

    // This function returns all content in year range [yearL, yearR].
    vector<Content> rangeQuery(vector<Content>& titles, int yearL, int yearR);

    // This function returns latest top k titles after sorting by year.
    vector<Content> topK(vector<Content>& titles, int k);
};
