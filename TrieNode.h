#ifndef TRIENODE_H
#define TRIENODE_H

#include <unordered_map>

class TrieNode {
public:
    std::unordered_map<char, TrieNode*> children;
    bool isEndOfWord;
    int frequency; // Key for ranking: higher frequency = better suggestion

    TrieNode();
    ~TrieNode(); // Important for cleaning up memory
};

#endif