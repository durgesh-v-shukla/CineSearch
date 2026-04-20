#include "LinearSearch.h"
using namespace std;

// This function compares two content records by year descending.
bool compareByYear(Content a, Content b) {
    if (a.releaseYear != b.releaseYear) {
        return a.releaseYear > b.releaseYear;
    }
    return a.title < b.title;
}

// This function does exact title search using full linear scan.
Content* LinearSearch::exactSearch(vector<Content>& titles, string target) {
    for (int i = 0; i < (int)titles.size(); i++) {
        if (titles[i].title == target) {
            return &titles[i];
        }
    }
    return nullptr;
}

// This function returns all titles starting with the given prefix.
vector<Content> LinearSearch::prefixSearch(vector<Content>& titles, string prefix) {
    vector<Content> results;
    for (int i = 0; i < (int)titles.size(); i++) {
        if (titles[i].title.size() >= prefix.size() &&
            titles[i].title.compare(0, prefix.size(), prefix) == 0) {
            results.push_back(titles[i]);
        }
    }
    return results;
}

// This function returns all content in year range [yearL, yearR].
vector<Content> LinearSearch::rangeQuery(vector<Content>& titles, int yearL, int yearR) {
    vector<Content> results;
    for (int i = 0; i < (int)titles.size(); i++) {
        if (titles[i].releaseYear >= yearL && titles[i].releaseYear <= yearR) {
            results.push_back(titles[i]);
        }
    }
    return results;
}

// This function returns latest top k titles after sorting by year.
vector<Content> LinearSearch::topK(vector<Content>& titles, int k) {
    vector<Content> arr = titles;
    sort(arr.begin(), arr.end(), compareByYear);
    if (k > (int)arr.size()) {
        k = (int)arr.size();
    }
    vector<Content> ans;
    for (int i = 0; i < k; i++) {
        ans.push_back(arr[i]);
    }
    return ans;
}
