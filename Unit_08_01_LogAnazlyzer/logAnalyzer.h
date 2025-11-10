#ifndef LOGANALYZER_H
#define LOGANALYZER_H

#include <map>
#include <string>
#include <vector>
#include <mutex>

using namespace std;

class LogAnalyzer {
private:
    map<string, int> keywordCounts;
    mutable mutex countMutex;

public:
   
    void analyzeFile(const string& filename, const vector<string>& keywords);

    
    void printSummary() const;
};

#endif 
