#pragma once
#include <ostream>
#include <memory>
#include <vector>
#include <string>
#include <pwd.h>
#include <grp.h>

// Namespace that contains methods to select specific inodes from specific paths
namespace INodeSelector {
    // Returns a unique_ptr on vector of strings in format "[f/d] <path>"
    // Every entry contains an absolute path to the iNode, that can be written to
    // by process with given username and groupname
    // In case of error returns nullptr, all error messages are printed in specified std::ostream
    std::unique_ptr<std::vector<std::string>> getWritableINodes(const char* path, const char* username, const char* groupname, std::ostream* errorStream = nullptr);
    // Returns a unique_ptr on vector of strings in format "[f/d] <path>"
    // Every entry contains an absolute path to the iNode, that can be written to
    // by process with given uid and gid
    // All error messages are printed in specified std::ostream
    std::unique_ptr<std::vector<std::string>> getWritableINodes(const char* path, uid_t uid, gid_t gid, std::ostream* errorStream = nullptr);
}