#pragma once
#include <ostream>
#include <iostream>
#include <memory>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>

// Namespace that contains methods that wrap Unix system calls,
// and automatically output error messages in specified ostream.
namespace UnixWrappers {
    // Retruns a pointer to passwd structure obtained from given username.
    // The pointer must not be deallocated
    // Returns nullptr on error
    struct passwd* getUserInfo(const char* username, std::ostream* errorStream = nullptr);
    // Retruns a pointer to group structure obtained from given groupname.
    // The pointer must not be deallocated
    // Returns nullptr on error
    struct group* getGroupInfo(const char* groupname, std::ostream* errorStream = nullptr);
    // Retruns a unique_ptr containing a stat strusture for the file at given path.
    // Returns nullptr on error
    std::unique_ptr<struct stat> getINodeStat(const char* path, std::ostream* errorStream = nullptr);
};