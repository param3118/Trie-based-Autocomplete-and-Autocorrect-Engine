#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <random>
#include <algorithm>
#include "Trie.h"

using namespace std;
using namespace std::chrono;

// Load dictionary function
void loadDictionaryFromCSV(Trie& trie, const std::string& filename) {
    ifstream file(filename);
    string line;
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }
    cout << "Loading dictionary for benchmark..." << endl;
    getline(file, line); // Skip header
    int count = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string word, countStr;
        if (getline(ss, word, ',') && getline(ss, countStr)) {
            try {
                long long freq = stoll(countStr);
                trie.insert(word, static_cast<int>(freq / 1000000));
                count++;
            } catch (...) {}
        }
    }
    file.close();
    cout << "Loaded " << count << " words." << endl;
}

// Generate random test cases - FIXED VARIABLE NAME
vector<string> generateTestCases(const vector<string>& prefixes, int count_per_prefix) {
    vector<string> testCases;
    default_random_engine generator;
    
    for (const auto& prefix : prefixes) {
        for (int i = 0; i < count_per_prefix; ++i) {
            testCases.push_back(prefix);
        }
    }
    
    // Shuffle to avoid any ordering effects
    shuffle(testCases.begin(), testCases.end(), generator);
    return testCases;
}

void runHardcoreBenchmark(Trie& trie) {
    cout << "\n=== HARDCORE BENCHMARK ===" << endl;
    cout << "Running 10,000 autocomplete queries..." << endl;
    
    // Test categories: common, medium, rare, and non-existent prefixes
    vector<string> prefixTypes = {"a", "the", "pro", "comp", "xyz", "unexist"};
    auto testCases = generateTestCases(prefixTypes, 1666); // ~1666 of each type for 10k total
    
    long long totalTime = 0;
    vector<long long> times;
    times.reserve(testCases.size());
    
    // WARMUP: Run first 1000 to cache everything
    cout << "Running warmup queries..." << endl;
    for (int i = 0; i < 1000; ++i) {
        trie.getAutocompleteSuggestions(testCases[i], 5);
    }
    
    // ACTUAL TIMED BENCHMARK
    cout << "Running timed benchmark..." << endl;
    auto start = high_resolution_clock::now();
    
    for (const auto& prefix : testCases) {
        auto queryStart = high_resolution_clock::now();
        auto suggestions = trie.getAutocompleteSuggestions(prefix, 5);
        auto queryEnd = high_resolution_clock::now();
        
        auto duration = duration_cast<microseconds>(queryEnd - queryStart).count();
        times.push_back(duration);
        totalTime += duration;
    }
    
    auto end = high_resolution_clock::now();
    auto totalDuration = duration_cast<milliseconds>(end - start).count();
    
    // Calculate statistics
    sort(times.begin(), times.end());
    long long avgTime = totalTime / times.size();
    long long p95 = times[static_cast<int>(times.size() * 0.95)];
    long long p99 = times[static_cast<int>(times.size() * 0.99)];
    long long maxTime = times.back();
    
    // RESULTS
    cout << "\nRESULTS (10,000 queries):" << endl;
    cout << "----------------------------------------" << endl;
    cout << "Total time: " << totalDuration << " ms" << endl;
    cout << "Average latency: " << avgTime << " μs" << endl;
    cout << "95th percentile: " << p95 << " μs" << endl;
    cout << "99th percentile: " << p99 << " μs" << endl;
    cout << "Worst-case: " << maxTime << " μs" << endl;
    cout << "QPS: " << (1000000 / avgTime) * 1000 << " queries/second" << endl;
    cout << "----------------------------------------" << endl;
    
    // Breakdown by prefix type
    cout << "\nLATENCY BY PREFIX TYPE:" << endl;
    cout << "----------------------------------------" << endl;
    for (const auto& prefix : prefixTypes) {
        long long sum = 0;
        int count = 0;
        for (const auto& testCase : testCases) {
            if (testCase == prefix) {
                auto queryStart = high_resolution_clock::now();
                trie.getAutocompleteSuggestions(prefix, 5);
                auto queryEnd = high_resolution_clock::now();
                sum += duration_cast<microseconds>(queryEnd - queryStart).count();
                count++;
            }
        }
        if (count > 0) {
            cout << prefix << ":\t" << (sum / count) << " μs avg" << endl;
        }
    }
}

int main() {
    Trie trie;
    loadDictionaryFromCSV(trie, "unigram_freq.csv");
    
    cout << "Press Enter to run hardcore benchmark...";
    cin.ignore();
    
    runHardcoreBenchmark(trie);
    
    cout << "\nBenchmark complete!" << endl;
    return 0;
}