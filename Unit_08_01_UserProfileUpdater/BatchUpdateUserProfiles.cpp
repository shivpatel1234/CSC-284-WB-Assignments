// Unit 9 Assignment 1: Batch Update User Profiles

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <chrono>

#include "json.hpp"   // nlohmann JSON single-header library

namespace fs = std::filesystem;
using json = nlohmann::json;
using namespace std;   

// Helper: create a timestamp string: YYYYMMDD_HHMMSS
std::string make_timestamp()
{
    using clock = std::chrono::system_clock;
    auto now = clock::now();
    
    std::time_t t = clock::to_time_t(now);
    std::tm tm{};

#ifdef _WIN32
    localtime_s(&tm, &t);
#else
    localtime_r(&t, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
    
    return oss.str();
}

// Helper: check if a string ends with a given suffix
bool ends_with(const std::string& s, const std::string& suffix)
{
    if (s.size() < suffix.size()) 
        
        return false;
    
    return std::equal(suffix.rbegin(), suffix.rend(), s.rbegin());
}

// Recursive JSON transformer
//  - Walks entire JSON tree
//  - For strings, applies replacement rules
void transform_json_value(json& j)
{
    if (j.is_object())
    {
        for (auto& [key, value] : j.items())
        {transform_json_value(value);
        }
    }
    else if (j.is_array())
    {
        for (auto& element : j)
        {transform_json_value(element);
        }
    }
    else if (j.is_string())
    {
        std::string s = j.get<std::string>();

        // Exact string replacements
        if (s == "enabled")
        {
            j = json{
                {"status", "enabled"},
                {"since", "2024-10-01"}
            };
            
            return;
        }
        else if (s == "disabled")
        {
            j = json{
                {"status", "disabled"},
                {"since", "2024-10-01"}
            };
            
            return;
        }
        else if (s == "manage_users")
        {
            j = json{
                {"permission", "manage_users"},
                {"granted_at", "2024-10-05"},
                {"level", "full"}
            };
            
            return;
        }
        else if (s == "view_content")
        {
            j = json{
                {"permission", "view_content"},
                {"granted_at", "2024-09-25"},
                {"level", "read-only"}
            };
            
            return;
        }

        // Email domain replacement
        const std::string oldDomain = "@company.com";
        const std::string newDomain = "@newcompany.com";
        
        if (ends_with(s, oldDomain))
        {
            std::string prefix = s.substr(0, s.size() - oldDomain.size());
            j = prefix + newDomain;
        }
    }
    // Other JSON types are left unchanged
}

// Process a single JSON file. Returns true on success, false on any error.
bool process_json_file(const fs::path& src, const fs::path& dst)
{
    try
    {
        std::ifstream in(src);
        
        if (!in)
        {
            std::cerr << "Error: Cannot open input file: " << src << "\n";
            
            return false;
        }

        json j;
        in >> j;      // may throw if file is not valid JSON
        in.close();

        // Apply transformations recursively
        transform_json_value(j);

        fs::create_directories(dst.parent_path());

        std::ofstream out(dst);
        
        if (!out)
        {
            std::cerr << "Error: Cannot open output file: " << dst << "\n";
            
            return false;
        }
        out << std::setw(4) << j << "\n";
        out.close();

        return true;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Warning: Failed to process JSON file '" << src
            << "': " << e.what() << "\n";
        
        return false;
    }
}


// Copy file as-is
// Used when JSON processing fails or for non-JSON files
void copy_file_as_is(const fs::path& src, const fs::path& dst)
{
    try
    {
        fs::create_directories(dst.parent_path());
        fs::copy_file(src, dst, fs::copy_options::overwrite_existing);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: Failed to copy file '" << src
            << "' to '" << dst << "': " << e.what() << "\n";
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <user_profiles_directory>\n";
        
        return 1;
    }

    fs::path inputRoot = argv[1];

    if (!fs::exists(inputRoot))
    {
        std::cerr << "Error: Input directory does not exist: " << inputRoot << "\n";
        
        return 1;
    }
    if (!fs::is_directory(inputRoot))
    {
        std::cerr << "Error: Provided path is not a directory: " << inputRoot << "\n";
        
        return 1;
    }

    // Build name for output directory
    std::string timestamp = make_timestamp();
    fs::path outputRoot = "user_profiles_updated_" + timestamp;

    try
    {
        fs::create_directories(outputRoot);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: Cannot create output directory '" << outputRoot
            << "': " << e.what() << "\n";
        
        return 1;
    }

    std::cout << "Input directory:  " << inputRoot << "\n";
    std::cout << "Output directory: " << outputRoot << "\n\n";

    // Recursively walk input directory
    for (const auto& entry : fs::recursive_directory_iterator(inputRoot))
    {
        const fs::path& srcPath = entry.path();
        fs::path relative = fs::relative(srcPath, inputRoot);
        fs::path dstPath = outputRoot / relative;

        if (entry.is_directory())
        {
            // Mirror directories
            try
            {
                fs::create_directories(dstPath);
            }
            catch (const std::exception& e)
            {
                std::cerr << "Warning: Cannot create directory '" << dstPath
                    << "': " << e.what() << "\n";
            }
        }
        else if (entry.is_regular_file())
        {
            if (srcPath.extension() == ".json")
            {
                std::cout << "Processing JSON: " << srcPath << "\n";
                bool ok = process_json_file(srcPath, dstPath);
                
                if (!ok)
                {
                    std::cout << "  -> Copying file as-is.\n";
                    copy_file_as_is(srcPath, dstPath);
                }
            }
            else
            {
                std::cout << "Copying non-JSON file: " << srcPath << "\n";
                copy_file_as_is(srcPath, dstPath);
            }
        }
        else
        {
            std::cerr << "Skipping non-regular entry: " << srcPath << "\n";
        }
    }

    std::cout << "\nDone.\n";
    
    return 0;
}
