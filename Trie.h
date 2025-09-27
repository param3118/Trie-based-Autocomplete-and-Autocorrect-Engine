#ifndef TRIE_H
#define TRIE_H

#include "TrieNode.h"
#include <string>
#include <vector>
#include <functional> // For std::function
#include <algorithm> // For std::min

// Define a type for a suggestion (word and its frequency)
struct Suggestion {
    std::string word;
    int frequency;
    int editDistance; // New field for autocorrect
    
    Suggestion() : word(""), frequency(0), editDistance(0) {}
    Suggestion(const std::string& w, int f) : word(w), frequency(f), editDistance(0) {}
    Suggestion(const std::string& w, int f, int d) : word(w), frequency(f), editDistance(d) {}
};

class Trie {
private:
    TrieNode* root;

    // Helper functions
    void findAllSuggestions(TrieNode* node, const std::string& prefix, std::vector<Suggestion>& suggestions);
    void collectAllWords(TrieNode* node, const std::string& prefix, std::vector<Suggestion>& allWords);
    int levenshteinDistance(const std::string& word1, const std::string& word2);

public:
    Trie();
    ~Trie();

    void insert(const std::string& word, int freq = 1);
    bool search(const std::string& word);
    std::vector<Suggestion> getAutocompleteSuggestions(const std::string& prefix, int k = 10);
    std::vector<Suggestion> getAutocorrectSuggestions(const std::string& word, int maxDistance = 2, int k = 5);
};

#endif