#include "Trie.h"
#include <algorithm> // For std::sort

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    delete root; // Triggers the recursive destructor in TrieNode
}

void Trie::insert(const std::string& word, int freq) {
    TrieNode* current = root;
    for (char c : word) {
        if (current->children.find(c) == current->children.end()) {
            current->children[c] = new TrieNode();
        }
        current = current->children[c];
    }
    current->isEndOfWord = true;
    current->frequency += freq; // Add to frequency for ranking
}

bool Trie::search(const std::string& word) {
    TrieNode* current = root;
    for (char c : word) {
        if (current->children.find(c) == current->children.end()) {
            return false;
        }
        current = current->children[c];
    }
    return current->isEndOfWord;
}

// Recursive function to collect all words starting with a given prefix
void Trie::findAllSuggestions(TrieNode* node, const std::string& prefix, std::vector<Suggestion>& suggestions) {
    if (node->isEndOfWord) {
        suggestions.emplace_back(prefix, node->frequency);
    }
    for (const auto& pair : node->children) {
        findAllSuggestions(pair.second, prefix + pair.first, suggestions);
    }
}

std::vector<Suggestion> Trie::getAutocompleteSuggestions(const std::string& prefix, int k) {
    std::vector<Suggestion> suggestions;
    TrieNode* current = root;

    // Traverse to the end of the prefix
    for (char c : prefix) {
        if (current->children.find(c) == current->children.end()) {
            return suggestions; // Return empty vector if prefix not found
        }
        current = current->children[c];
    }

    // Collect all words from this node onward
    findAllSuggestions(current, prefix, suggestions);

    // Sort suggestions by frequency (highest first)
    std::sort(suggestions.begin(), suggestions.end(),
              [](const Suggestion& a, const Suggestion& b) {
                  return a.frequency > b.frequency; // Descending order
              });

    // Return top k suggestions
    if (suggestions.size() > k) {
        suggestions.erase(suggestions.begin() + k, suggestions.end());
    }
    return suggestions;
}

// Helper function to calculate Levenshtein Distance (Edit Distance)
int Trie::levenshteinDistance(const std::string& word1, const std::string& word2) {
    int m = word1.length();
    int n = word2.length();
    
    // Create a DP table to store edit distances
    std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
    
    // Initialize DP table
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i; // i deletions
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j; // j insertions
    }
    
    // Fill the DP table
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (word1[i - 1] == word2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1]; // No cost
            } else {
                dp[i][j] = 1 + std::min({dp[i - 1][j],    // Deletion
                                         dp[i][j - 1],    // Insertion
                                         dp[i - 1][j - 1]}); // Substitution
            }
        }
    }
    
    return dp[m][n];
}

// Function to collect all words in the trie for autocorrect
void Trie::collectAllWords(TrieNode* node, const std::string& prefix, std::vector<Suggestion>& allWords) {
    if (node->isEndOfWord) {
        allWords.emplace_back(prefix, node->frequency);
    }
    for (const auto& pair : node->children) {
        collectAllWords(pair.second, prefix + pair.first, allWords);
    }
}

// Autocorrect function implementation
std::vector<Suggestion> Trie::getAutocorrectSuggestions(const std::string& word, int maxDistance, int k) {
    std::vector<Suggestion> allWords;
    std::vector<Suggestion> suggestions;
    
    // Collect all words from the trie
    collectAllWords(root, "", allWords);
    
    // Calculate edit distance for each word and filter
    for (const auto& suggestion : allWords) {
        // Quick length check to avoid unnecessary calculations
        if (abs(static_cast<int>(word.length()) - static_cast<int>(suggestion.word.length())) <= maxDistance) {
            int distance = levenshteinDistance(word, suggestion.word);
            if (distance <= maxDistance) {
                suggestions.emplace_back(suggestion.word, suggestion.frequency, distance);
            }
        }
    }
    
    // Sort by edit distance first, then by frequency
    std::sort(suggestions.begin(), suggestions.end(),
              [](const Suggestion& a, const Suggestion& b) {
                  if (a.editDistance != b.editDistance) {
                      return a.editDistance < b.editDistance;
                  }
                  return a.frequency > b.frequency;
              });
    
    // Return top k suggestions
    if (suggestions.size() > k) {
        suggestions.erase(suggestions.begin() + k, suggestions.end());
    }
    
    return suggestions;
}