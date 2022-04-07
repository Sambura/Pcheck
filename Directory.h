#pragma once
#include <dirent.h>
#include <vector>
#include <string>
#include <ostream>
#include <memory>

// Namespace that contains methods for work with directories
namespace Directory {
    // Returns true, if path is '.' or '..'
    bool isSelfOrParent(const char* path);
    // Returns a vector of entries from given directory, except '.' and '..' entries.
    // Returns nullptr in case of error, and prints error in provided errorStream
    std::unique_ptr<std::vector<std::string>> getDirectoryEntries(const char* path, std::ostream* errorStream = nullptr);
};