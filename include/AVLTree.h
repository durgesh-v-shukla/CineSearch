#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "Content.h"
using namespace std;

struct AVLNode {
    Content data;
    AVLNode* left;
    AVLNode* right;
    int height;
};

class AVLTree {
private:
    AVLNode* root;
    int nodeCount;

    // This function returns node height or 0 if null.
    int getHeight(AVLNode* node);

    // This function returns balance factor.
    int getBalance(AVLNode* node);

    // This function creates a new AVL node.
    AVLNode* createNode(Content c);

    // This function performs right rotation.
    AVLNode* rotateRight(AVLNode* y);

    // This function performs left rotation.
    AVLNode* rotateLeft(AVLNode* x);

    // This function inserts content and balances tree.
    AVLNode* insertNode(AVLNode* node, Content c);

    // This function searches title in BST way.
    AVLNode* searchNode(AVLNode* node, string title);

    // This function frees all AVL nodes.
    void clearTree(AVLNode* node);

public:
    // This constructor initializes empty AVL tree.
    AVLTree();

    // This destructor frees tree memory.
    ~AVLTree();

    // This function builds AVL index from titles.
    void buildIndex(vector<Content>& titles);

    // This function runs exact title search.
    Content* exactSearch(string title);

    // This function returns number of AVL nodes.
    int size();
};
