#include "Trie.h"
using namespace std;

// This constructor initializes empty trie root.
Trie::Trie() {
    root = new TrieNode();
    totalTitles = 0;
    allTitles = nullptr;
}

// This destructor frees trie nodes.
Trie::~Trie() {
    clearTrie(root);
}

// This function recursively frees trie memory.
void Trie::clearTrie(TrieNode* node) {
    if (node == nullptr) {
        return;
    }
    for (int i = 0; i < 26; i++) {
        clearTrie(node->children[i]);
    }
    delete node;
}

// This function maps letter to index 0..25.
int Trie::charIdx(char c) {
    return tolower((unsigned char)c) - 'a';
}

// This function inserts one title index into prefix buckets.
void Trie::insertIndex(int idx) {
    if (allTitles == nullptr) {
        return;
    }

    TrieNode* curr = root;
    string s = (*allTitles)[idx].title;

    for (int i = 0; i < (int)s.size(); i++) {
        char ch = s[i];
        if (!isalpha((unsigned char)ch)) {
            continue;
        }

        int id = charIdx(ch);
        if (id < 0 || id >= 26) {
            continue;
        }

        if (curr->children[id] == nullptr) {
            curr->children[id] = new TrieNode();
        }

        curr = curr->children[id];
        curr->titleIndexes.push_back(idx);
    }

    curr->isEnd = true;
    curr->content = (*allTitles)[idx];
}

// This function builds trie from all titles.
void Trie::buildIndex(vector<Content>& titles) {
    allTitles = &titles;
    for (int i = 0; i < (int)titles.size(); i++) {
        insertIndex(i);
    }
    totalTitles = (int)titles.size();
}

// This function inserts one title in trie.
void Trie::insertTitle(Content c) {
    // This path is kept for compatibility if someone inserts manually.
    TrieNode* curr = root;
    for (int i = 0; i < (int)c.title.size(); i++) {
        char ch = c.title[i];
        if (!isalpha((unsigned char)ch)) {
            continue;
        }
        int idx = charIdx(ch);
        if (idx < 0 || idx >= 26) {
            continue;
        }
        if (curr->children[idx] == nullptr) {
            curr->children[idx] = new TrieNode();
        }
        curr = curr->children[idx];
    }
    curr->isEnd = true;
    curr->content = c;
    totalTitles++;
}

// This function returns all title records matching prefix.
vector<Content> Trie::prefixSearch(string prefix) {
    TrieNode* curr = root;
    for (int i = 0; i < (int)prefix.size(); i++) {
        char ch = prefix[i];
        if (!isalpha((unsigned char)ch)) {
            continue;
        }
        int idx = charIdx(ch);
        if (idx < 0 || idx >= 26) {
            continue;
        }
        if (curr->children[idx] == nullptr) {
            return vector<Content>();
        }
        curr = curr->children[idx];
    }

    vector<Content> results;
    if (allTitles != nullptr) {
        for (int i = 0; i < (int)curr->titleIndexes.size(); i++) {
            int id = curr->titleIndexes[i];
            if (id >= 0 && id < (int)allTitles->size()) {
                results.push_back((*allTitles)[id]);
            }
        }
    }
    return results;
}

// This function returns total inserted titles.
int Trie::size() {
    return totalTitles;
}
