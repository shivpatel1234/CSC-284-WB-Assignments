#include "LogAnalyzer.h"
#include <iostream>
#include <filesystem>
#include <thread>
#include <vector>
#include <cctype>
#include <algorithm>

using namespace std;
namespace fs = filesystem;

static  vector <string> listLogFiles(const string& dir) {
    vector <string> files;

    try {
        for (const auto& entry : fs::directory_iterator(dir)) {
            
            if (!entry.is_regular_file()) 
                
                continue;
            
            const auto& p = entry.path();
            if (!p.has_extension()) 
                
                continue;

            string ext = p.extension().string();
            transform(ext.begin(), ext.end(), ext.begin(),
                [](unsigned char c) { return static_cast<char>(tolower(c)); });

            if (ext == ".log") {
                
                files.push_back(p.string());
            }
        }
    }
    catch (const exception& e) {
        cerr << "Directory error: " << e.what() << "\n";
    }

    return files;
}

int main(int argc, char* argv[]) {
    
    try {
        
        if (argc < 2) {
            cerr << "Usage: " << argv[0] << " <log_directory>\n";
            
            return 1;
        }

        const string folderPath = argv[1];

       
        vector<string> keywords = {
            "[WARN]", "[ERROR]", "[FATAL]", "[INFO]", "[DEBUG]", "[TRACE]"
        };

        LogAnalyzer analyzer;
        vector<thread> threads;
        
        cout << "Analyzing folder: " << folderPath << "\n\n";

      
        vector<string> logFiles = listLogFiles(folderPath);

        for (const auto& file : logFiles) {
            
            cout << "Processing: " << fs::path(file).filename().string() << "\n";

            threads.emplace_back([&analyzer, &keywords, file]() {
                analyzer.analyzeFile(file, keywords);
                });
        }

       
        for (auto& t : threads) t.join();

        
        analyzer.printSummary();

        
        cout << "[\"WARN\", \"ERROR\", \"FATAL\", \"INFO\", \"DEBUG\", \"TRACE\"\n";
        cout << "-----------------------\n\n";

        cout << "Analysis complete. Processed " << logFiles.size() << " files.\n";
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << "\n";
        
        return 2;
    }
    catch (...) {
        cerr << "Unknown error occurred.\n";
        
        return 3;
    }

    return 0;
}
