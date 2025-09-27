#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <cmath>
#include "Trie.h"

using namespace std;

// Function to load words and frequencies from the Kaggle CSV file
void loadDictionaryFromCSV(Trie& trie, const std::string& filename) {
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    cout << "Loading massive dictionary from Google Web Corpus... This will take a minute." << endl;
    
    // Skip the header line if it exists ("word,count")
    getline(file, line);
    
    int count = 0;
    long long totalWordsProcessed = 0;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string word;
        string countStr;
        
        // Parse the line: word,count
        if (getline(ss, word, ',') && getline(ss, countStr)) {
            try {
                long long frequency = stoll(countStr);
                int compressedFrequency = static_cast<int>(frequency / 1000000);
                
                trie.insert(word, compressedFrequency);
                count++;
                totalWordsProcessed += frequency;

            } catch (const exception& e) {
                // Skip lines that can't be parsed quietly
            }
        }
        if (count % 50000 == 0) {
            cout << "Loaded " << count << " words so far..." << endl;
        }
    }
    file.close();
    cout << "--------------------------------------------------" << endl;
    cout << "Trie construction complete!" << endl;
    cout << "Unique words loaded: " << count << endl;
    cout << "Total word occurrences represented: " << totalWordsProcessed << endl;
    cout << "--------------------------------------------------" << endl;
}

// Function to get autocomplete suggestions with timing
vector<Suggestion> getSuggestionsWithTiming(Trie& trie, const string& prefix, int k, double& timeTaken) {
    auto start = chrono::high_resolution_clock::now();
    vector<Suggestion> suggestions = trie.getAutocompleteSuggestions(prefix);
    auto end = chrono::high_resolution_clock::now();
    
    timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    // Ensure we don't return more than k suggestions
    if (suggestions.size() > k) {
        suggestions.erase(suggestions.begin() + k, suggestions.end());
    }
    return suggestions;
}

void displayMenu() {
    cout << "\n=== Trie Autocomplete System ===" << endl;
    cout << "1. Test Autocomplete" << endl;
    cout << "2. Test Autocorrect" << endl;  // NEW
    cout << "3. Run Performance Benchmark" << endl;
    cout << "4. Search for a Specific Word" << endl;
    cout << "5. Exit" << endl;
    cout << "Choose an option (1-5): ";
}

// NEW FUNCTION: Test autocorrect
void testAutocorrect(Trie& trie) {
    string word;
    int maxDistance, k;
    
    cout << "\n--- Autocorrect Test ---" << endl;
    cout << "Enter misspelled word: ";
    cin >> word;
    
    cout << "Enter maximum edit distance (1-3): ";
    cin >> maxDistance;
    
    cout << "Enter number of suggestions (K): ";
    cin >> k;
    
    if (maxDistance <= 0 || k <= 0) {
        cout << "Error: Values must be positive numbers." << endl;
        return;
    }
    
    double timeTaken;
    auto start = chrono::high_resolution_clock::now();
    vector<Suggestion> suggestions = trie.getAutocorrectSuggestions(word, maxDistance, k);
    auto end = chrono::high_resolution_clock::now();
    
    timeTaken = chrono::duration_cast<chrono::microseconds>(end - start).count();
    
    cout << "\nAutocorrect suggestions for '" << word << "':" << endl;
    cout << "Search time: " << timeTaken << " microseconds (" << timeTaken/1000.0 << " ms)" << endl;
    
    if (suggestions.empty()) {
        cout << "No suggestions found within edit distance " << maxDistance << "." << endl;
    } else {
        for (size_t i = 0; i < suggestions.size(); i++) {
            cout << i+1 << ". " << suggestions[i].word 
                 << " (edit distance: " << suggestions[i].editDistance 
                 << ", frequency: " << suggestions[i].frequency << ")" << endl;
        }
    }
}
void testAutocomplete(Trie& trie) {
    string prefix;
    int k;
    
    cout << "\n--- Autocomplete Test ---" << endl;
    cout << "Enter prefix to search: ";
    cin >> prefix;
    
    cout << "Enter number of suggestions (K): ";
    cin >> k;
    
    if (k <= 0) {
        cout << "Error: K must be a positive number." << endl;
        return;
    }
    
    double timeTaken;
    vector<Suggestion> suggestions = getSuggestionsWithTiming(trie, prefix, k, timeTaken);
    
    cout << "\nResults for prefix '" << prefix << "' (Top " << k << " suggestions):" << endl;
    cout << "Search time: " << timeTaken << " microseconds (" << timeTaken/1000.0 << " ms)" << endl;
    
    if (suggestions.empty()) {
        cout << "No suggestions found for the given prefix." << endl;
    } else {
        for (size_t i = 0; i < suggestions.size(); i++) {
            cout << i+1 << ". " << suggestions[i].word 
                 << " (frequency: " << suggestions[i].frequency << ")" << endl;
        }
    }
}

void runBenchmark(Trie& trie) {
    cout << "\n--- Performance Benchmark ---" << endl;
    
    vector<string> testPrefixes = {"a", "the", "pro", "comput", "test", "artific", "ze", "what", "how", "why"};
    long long totalTime = 0;

    cout << "Testing 10 common prefixes..." << endl;
    for (const auto& prefix : testPrefixes) {
        double timeTaken;
        getSuggestionsWithTiming(trie, prefix, 5, timeTaken);
        totalTime += timeTaken;
        cout << " - '" << prefix << "': " << timeTaken << " μs" << endl;
    }

    double averageTime = totalTime / static_cast<double>(testPrefixes.size());
    cout << "\nAverage autocomplete time: " << averageTime << " microseconds (" 
         << averageTime/1000.0 << " milliseconds)" << endl;
}

void searchWord(Trie& trie) {
    string word;
    
    cout << "\n--- Word Search ---" << endl;
    cout << "Enter word to search: ";
    cin >> word;
    
    if (trie.search(word)) {
        cout << "✓ Word '" << word << "' found in the dictionary!" << endl;
    } else {
        cout << "✗ Word '" << word << "' not found in the dictionary." << endl;
    }
}

int main() {
    Trie trie;

    // Load the dictionary
    loadDictionaryFromCSV(trie, "unigram_freq.csv");

    int choice;
    do {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
    case 1:
        testAutocomplete(trie);
        break;
    case 2:  // NEW
        testAutocorrect(trie);
        break;
    case 3:
        runBenchmark(trie);
        break;
    case 4:
        searchWord(trie);
        break;
    case 5:
        cout << "Exiting program. Goodbye!" << endl;
        break;
    default:
        cout << "Invalid choice. Please try again." << endl;
        break;
}
        
        // Clear input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
    } while (choice != 4);

    return 0;
}