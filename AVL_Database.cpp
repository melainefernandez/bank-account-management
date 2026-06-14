// AVL_Database.cpp
#include "AVL_Database.hpp"
#include <algorithm>
#include <cmath>

Record::Record(const std::string& k, int v) : key(k), value(v) {}

AVLNode::AVLNode(Record* r) : record(r), left(nullptr), right(nullptr), height(1) {}

AVLTree::AVLTree() : root(nullptr), nodeCount(0), searchComparisonCount(0) {}

int AVLTree::height(AVLNode* node) {
    return node ? node->height : 0;
}

void AVLTree::updateHeight(AVLNode* node) {
    if (node) {
        node->height = 1 + std::max(height(node->left), height(node->right));
    }
}

int AVLTree::getBalance(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

/*
Can be used to balance AVL trees
*/
AVLNode* AVLTree::rotateRight(AVLNode* y) {
    AVLNode* x = y->left;       // x becomes new root of the rotated subtree
    AVLNode* T2 = x->right;     // T2 is the right child of x, which becomes left child of y
    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    updateHeight(y);
    updateHeight(x);

    // Return new root
    return x;
}

/*
Can be used to balance AVL trees
*/
AVLNode* AVLTree::rotateLeft(AVLNode* x) {
    // Perform left rotation
    AVLNode* y = x->right;         // y becomes new root of the rotated subtree
    AVLNode* T2 = y->left;         // T2 is the left child of y, which becomes right child of x

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    updateHeight(x);
    updateHeight(y);

    // Return new root
    return y;


}

AVLNode* AVLTree::insertHelper(AVLNode* node, Record* record) {
    // Step 1: Perform standard BST insertion
    if (!node) {
        return new AVLNode(record);
    }

    if (record->value < node->record->value) {
        node->left = insertHelper(node->left, record);
    } else if (record->value > node->record->value) {
        node->right = insertHelper(node->right, record);
    } else {
        // If the value is equal, break tie using the key
        if (record->key < node->record->key) {
            node->left = insertHelper(node->left, record);
        } else if (record->key > node->record->key) {
            node->right = insertHelper(node->right, record);
        } else {
            // Duplicate key and value — discard or handle as needed
            return node;
        }
    }

    // Step 2: Update the height of this ancestor node
    updateHeight(node);

    // Step 3: Get the balance factor and apply rotations if unbalanced
    int balance = getBalance(node);

    // Left Left Case
    if (balance > 1 && record->value < node->left->record->value)
        return rotateRight(node);

    // Right Right Case
    if (balance < -1 && record->value > node->right->record->value)
        return rotateLeft(node);

    // Left Right Case
    if (balance > 1 && record->value > node->left->record->value) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right Left Case
    if (balance < -1 && record->value < node->right->record->value) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // Node is balanced
    return node;
}




void AVLTree::insert(Record* record) {
    root = insertHelper(root, record);
    nodeCount++;
}

void AVLTree::deleteNode(const std::string& key, int value) {
    root = deleteHelper(root, key, value);
}

// Helper inside the same file (you already have most of it done)
AVLNode* AVLTree::deleteHelper(AVLNode* node, const std::string& key, int value) {
    if (!node) return nullptr;

    if (value < node->record->value) {
        node->left = deleteHelper(node->left, key, value);
    } else if (value > node->record->value) {
        node->right = deleteHelper(node->right, key, value);
    } else {
        if (key < node->record->key) {
            node->left = deleteHelper(node->left, key, value);
        } else if (key > node->record->key) {
            node->right = deleteHelper(node->right, key, value);
        } else {
            // Node to be deleted found
            if (!node->left || !node->right) {
                AVLNode* temp = node->left ? node->left : node->right;
                delete node->record;
                delete node;
                nodeCount--;
                return temp;
            } else {
                // Node with two children
                AVLNode* temp = node->right;
                while (temp->left)
                    temp = temp->left;

                // Copy successor data
                node->record->key = temp->record->key;
                node->record->value = temp->record->value;

                node->right = deleteHelper(node->right, temp->record->key, temp->record->value);
            }
        }
    }

    updateHeight(node);
    int balance = getBalance(node);

    // Balance the node if needed
    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);

    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node);

    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}





Record* AVLTree::search(const std::string& key, int value) {
    searchComparisonCount = 0;  // Reset before each search
    AVLNode* result = searchHelper(root, key, value);
    if (result) return result->record;
    return new Record("", 0);  // Return an empty/default record if not found
}

AVLNode* AVLTree::searchHelper(AVLNode* node, const std::string& key, int value) const {
    if (!node) return nullptr;

    searchComparisonCount++;  // Track number of comparisons

    if (value < node->record->value) {
        return searchHelper(node->left, key, value);
    } else if (value > node->record->value) {
        return searchHelper(node->right, key, value);
    } else {
        // Tie-break using key
        if (key < node->record->key) {
            return searchHelper(node->left, key, value);
        } else if (key > node->record->key) {
            return searchHelper(node->right, key, value);
        } else {
            // Match found
            return node;
        }
    }
}


// IndexedDatabase Implementation
void IndexedDatabase::insert(Record* record) {
    index.insert(record);
}

Record* IndexedDatabase::search(const std::string& key, int value) {
    return index.search(key, value);
}

void IndexedDatabase::deleteRecord(const std::string& key, int value) {
    index.deleteNode(key, value);
}



/* RangeQuery Hints

1. Base: if (!node) return   
2. Key Traversal Logic:
   - If value >= start: check left subtree
   - If start <= value <= end: add to result
   - If value <= end: check right subtree
*/




std::vector<Record*> IndexedDatabase::rangeQuery(int start, int end) {
    std::vector<Record*> result;
    rangeQueryHelper(index.root, start, end, result);
    return result;
}

void IndexedDatabase::rangeQueryHelper(AVLNode* node, int start, int end, std::vector<Record*>& result) const {
    if (!node) return;

    // If current node’s value is greater than or equal to start, explore left subtree
    if (node->record->value >= start) {
        rangeQueryHelper(node->left, start, end, result);
    }

    // If current node’s value is within the range, add it to result
    if (node->record->value >= start && node->record->value <= end) {
        result.push_back(node->record);
    }

    // If current node’s value is less than or equal to end, explore right subtree
    if (node->record->value <= end) {
        rangeQueryHelper(node->right, start, end, result);
    }
}



void IndexedDatabase::clearHelper(AVLNode* node) {
    if (!node) return;
    clearHelper(node->left);
    clearHelper(node->right);
    delete node->record;
    delete node;
}

void IndexedDatabase::clearDatabase() {
    clearHelper(index.root);
    index.root = nullptr;
}

int IndexedDatabase::calculateHeight(AVLNode* node) const {
    if (!node) return 0;
    return 1 + std::max(calculateHeight(node->left), calculateHeight(node->right));
}

int IndexedDatabase::getTreeHeight() const {
    return calculateHeight(index.root);
}

int IndexedDatabase::getSearchComparisons(const std::string& key, int value) {
    search(key, value);
    return index.getLastSearchComparisons();
}

AVLNode* AVLTree::minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current && current->left != nullptr)
        current = current->left;
    return current;
}
