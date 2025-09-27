#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include "Trie.h"

using namespace std;

// Function to load the dictionary (same as your main.cpp)
void loadDictionaryFromCSV(Trie& trie, const std::string& filename) {
    ifstream file(filename);
    string line;

    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    cout << "Loading massive dictionary from Google Web Corpus... This will take a minute." << endl;
    
    // Skip the header line
    getline(file, line);
    
    int count = 0;
    long long totalWordsProcessed = 0;
    
    while (getline(file, line)) {
        stringstream ss(line);
        string word;
        string countStr;
        
        if (getline(ss, word, ',') && getline(ss, countStr)) {
            try {
                long long frequency = stoll(countStr);
                int compressedFrequency = static_cast<int>(frequency / 1000000);
                
                trie.insert(word, compressedFrequency);
                count++;
                totalWordsProcessed += frequency;

            } catch (const exception& e) {
                // Skip invalid lines
            }
        }
        if (count % 50000 == 0) {
            cout << "Loaded " << count << " words so far..." << endl;
        }
    }
    file.close();
    cout << "--------------------------------------------------" << endl;
    cout << "Trie construction complete! " << count << " words loaded." << endl;
    cout << "--------------------------------------------------" << endl;
}

void runComprehensiveBenchmark(Trie& trie) {
    cout << "\n=== COMPREHENSIVE BENCHMARK ===" << endl;
    
    // Test autocomplete with various scenarios
    vector<string> testCases = {
        "a", "the", "pro", "artific", "comput", "unexpect", "zyx"
    };
    
    cout << "\n1. AUTOCOMPLETE PERFORMANCE:" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Prefix\t\tTime (μs)\tSuggestions" << endl;
    cout << "----------------------------------------" << endl;
    
    long long totalTime = 0;
    int testCount = 0;
    
    for (const auto& prefix : testCases) {
        // Run multiple times to get average
        long long sumTime = 0;
        const int runs = 100; // Run each test 100 times for better average
        
        for (int i = 0; i < runs; i++) {
            auto start = chrono::high_resolution_clock::now();
            auto suggestions = trie.getAutocompleteSuggestions(prefix, 5);
            auto end = chrono::high_resolution_clock::now();
            sumTime += chrono::duration_cast<chrono::microseconds>(end - start).count();
        }
        
        long long avgTime = sumTime / runs;
        auto suggestions = trie.getAutocompleteSuggestions(prefix, 5);
        
        cout << prefix;
        if (prefix.length() < 8) cout << "\t"; // Formatting
        cout << "\t" << avgTime << " μs\t\t" << suggestions.size() << endl;
        
        totalTime += avgTime;
        testCount++;
    }
    cout << "----------------------------------------" << endl;
    cout << "Average autocomplete time: " << totalTime/testCount << " μs" << endl;
    
    // Test autocorrect
    cout << "\n2. AUTOCORRECT PERFORMANCE:" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Word\t\tTime (ms)\tSuggestions" << endl;
    cout << "----------------------------------------" << endl;
    
    vector<string> misspelled = {"artifical", "computar", "proogram", "theire", "recieve"};
    totalTime = 0;
    testCount = 0;
    
    for (const auto& word : misspelled) {
        auto start = chrono::high_resolution_clock::now();
        auto suggestions = trie.getAutocorrectSuggestions(word, 2, 3);
        auto end = chrono::high_resolution_clock::now();
        
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        totalTime += duration;
        testCount++;
        
        cout << word;
        if (word.length() < 8) cout << "\t"; // Formatting
        cout << "\t" << duration << " ms\t\t" << suggestions.size() << endl;
        
        // Show top suggestion
        if (!suggestions.empty()) {
            cout << "  → Top suggestion: '" << suggestions[0].word << "' (edit distance: " 
                 << suggestions[0].editDistance << ")" << endl;
        }
    }
    cout << "----------------------------------------" << endl;
    cout << "Average autocorrect time: " << totalTime/testCount << " ms" << endl;
    
    // Memory usage estimation (simple approach)
    cout << "\n3. MEMORY USAGE ESTIMATION:" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Approximate memory usage: ~100-200 MB" << endl;
    cout << "(Based on 333,000 words × ~300-600 bytes per word in Trie)" << endl;
}

int main() {
    Trie trie;
    
    cout << "Loading dictionary for benchmarking..." << endl;
    loadDictionaryFromCSV(trie, "unigram_freq.csv");
    
    // Wait for user to start benchmark
    cout << "\nPress Enter to start benchmarking...";
    cin.ignore();
    
    runComprehensiveBenchmark(trie);
    
    cout << "\nBenchmark complete! Press Enter to exit...";
    cin.ignore();
    
    return 0;
}