#pragma once
#include <vector>
#include <string>
#include <cctype>
#include "Content.h"
using namespace std;

struct TrieNode {
    TrieNode* children[26];
    bool isEnd;
    Content content;
    vector<int> titleIndexes;

    TrieNode() {
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
        isEnd = false;
    }
};

class Trie {
private:
    TrieNode* root;
    int totalTitles;
    vector<Content>* allTitles;

    // This function maps letter to index 0..25.
    int charIdx(char c);

    // This function inserts one title index into prefix buckets.
    void insertIndex(int idx);

    // This function recursively frees trie memory.
    void clearTrie(TrieNode* node);

public:
    // This constructor initializes empty trie root.
    Trie();

    // This destructor frees trie nodes.
    ~Trie();

    // This function builds trie from all titles.
    void buildIndex(vector<Content>& titles);

    // This function inserts one title in trie.
    void insertTitle(Content c);

    // This function returns all title records matching prefix.
    vector<Content> prefixSearch(string prefix);

    // This function returns total inserted titles.
    int size();
};
