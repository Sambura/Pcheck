#pragma once
#include <sys/stat.h>
#include <sys/types.h>

// Class that has static methods to check iNode permissions
class PermissionChecker {
public:
	// Determines, whether a process launched with the given uid 
	// and gid has permission to read the given iNode
	// Returns true, if uid = 0 (root)
	static bool isReadable(const struct stat& iNode, uid_t uid, gid_t gid);
	
	// Determines, whether a process launched with the given uid 
	// and gid has permission to write to the given iNode
	// Returns true, if uid = 0 (root)
	static bool isWritable(const struct stat& iNode, uid_t uid, gid_t gid);
	
	// Determines, whether a process launched with the given uid 
	// and gid has permission to execute/search the given iNode
	// Returns true, if uid = 0 (root)
	static bool isExecutable(const struct stat& iNode, uid_t uid, gid_t gid);
	
	PermissionChecker() = delete;
};