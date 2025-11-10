#include "LogAnalyzer.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

// Count all (possibly multiple) occurrences of 'needle' within 'haystack'
static int countAll(const string& haystack, const string& needle) {
    if (needle.empty()) return 0;
    int cnt = 0;
    string::size_type pos = 0;
    while ((pos = haystack.find(needle, pos)) != string::npos) {
        ++cnt;
        pos += needle.size();
    }
    return cnt;
}

void LogAnalyzer::analyzeFile(const string& filename, const vector<string>& keywords) {
    ifstream in(filename.c_str());
    if (!in) {
        cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    
    map<string, int> localCounts;

    string line;
    while (getline(in, line)) {
        for (const auto& key : keywords) {
            int hits = countAll(line, key);
            if (hits > 0) {
                localCounts[key] += hits;
            }
        }
    }

   
    {
        lock_guard<mutex> lock(countMutex);
        for (const auto& kv : localCounts) {
            keywordCounts[kv.first] += kv.second;
        }
    }
}

void LogAnalyzer::printSummary() const {
    cout << "\n--- Keyword Summary ---\n";

  
    const string order[] = { "[WARN]", "[ERROR]", "[FATAL]", "[INFO]", "[DEBUG]", "[TRACE]" };

    for_each(begin(order), end(order), [this](const string& key) {
        int value = 0;
        {
            lock_guard<mutex> lock(countMutex);
            auto it = keywordCounts.find(key);
            if (it != keywordCounts.end()) value = it->second;
        }

        if (key == "[WARN]")  cout << "WARN: " << value << "\n";
        else if (key == "[ERROR]") cout << "ERROR: " << value << "\n";
        else if (key == "[FATAL]") cout << "FATAL: " << value << "\n";
        else if (key == "[INFO]")  cout << "INFO: " << value << "\n";
        else if (key == "[DEBUG]") cout << "DEBUG: " << value << "\n";
        else if (key == "[TRACE]") cout << "TRACE: " << value << "\n";
        });

    cout << "-----------------------\n";
}
