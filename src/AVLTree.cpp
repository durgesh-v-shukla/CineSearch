#include "AVLTree.h"
using namespace std;

// This constructor initializes empty AVL tree.
AVLTree::AVLTree() {
    root = nullptr;
    nodeCount = 0;
}

// This destructor frees tree memory.
AVLTree::~AVLTree() {
    clearTree(root);
}

// This function frees all AVL nodes.
void AVLTree::clearTree(AVLNode* node) {
    if (node == nullptr) {
        return;
    }
    clearTree(node->left);
    clearTree(node->right);
    delete node;
}

// This function returns node height or 0 if null.
int AVLTree::getHeight(AVLNode* node) {
    if (node == nullptr) {
        return 0;
    }
    return node->height;
}

// This function returns balance factor.
int AVLTree::getBalance(AVLNode* node) {
    if (node == nullptr) {
        return 0;
    }
    return getHeight(node->left) - getHeight(node->right);
}

// This function creates a new AVL node.
AVLNode* AVLTree::createNode(Content c) {
    AVLNode* node = new AVLNode;
    node->data = c;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;
    return node;
}

// This function performs right rotation.
AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* temp = x->right;

    x->right = y;
    y->left = temp;

    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

// This function performs left rotation.
AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* temp = y->left;

    y->left = x;
    x->right = temp;

    x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// This function inserts content and balances tree.
AVLNode* AVLTree::insertNode(AVLNode* node, Content c) {
    if (node == nullptr) {
        nodeCount++;
        return createNode(c);
    }

    if (c.title < node->data.title) {
        node->left = insertNode(node->left, c);
    } else {
        // Duplicate titles go to right side for stability.
        node->right = insertNode(node->right, c);
    }

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
    int bal = getBalance(node);

    if (bal > 1 && c.title < node->left->data.title) {
        return rotateRight(node);
    }
    if (bal < -1 && c.title > node->right->data.title) {
        return rotateLeft(node);
    }
    if (bal > 1 && c.title > node->left->data.title) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (bal < -1 && c.title < node->right->data.title) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// This function searches title in BST way.
AVLNode* AVLTree::searchNode(AVLNode* node, string title) {
    if (node == nullptr) {
        return nullptr;
    }
    if (node->data.title == title) {
        return node;
    }
    if (title < node->data.title) {
        return searchNode(node->left, title);
    }
    return searchNode(node->right, title);
}

// This function builds AVL index from titles.
void AVLTree::buildIndex(vector<Content>& titles) {
    for (int i = 0; i < (int)titles.size(); i++) {
        root = insertNode(root, titles[i]);
    }
}

// This function runs exact title search.
Content* AVLTree::exactSearch(string title) {
    AVLNode* found = searchNode(root, title);
    if (found == nullptr) {
        return nullptr;
    }
    return &found->data;
}

// This function returns number of AVL nodes.
int AVLTree::size() {
    return nodeCount;
}
