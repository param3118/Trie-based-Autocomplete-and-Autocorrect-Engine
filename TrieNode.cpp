#include "TrieNode.h"
#include <iostream>

TrieNode::TrieNode() {
    isEndOfWord = false;
    frequency = 0;
}

// Crucial for preventing memory leaks in C++
TrieNode::~TrieNode() {
    for (auto& pair : children) {
        delete pair.second; // Recursively delete all child nodes
    }
}