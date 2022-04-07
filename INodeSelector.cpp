#include "INodeSelector.h"
#include "PermissionChecker.h"
#include "UnixWrappers.h"
#include "Directory.h"
#include <cstring>

using namespace std;

unique_ptr<vector<string>> INodeSelector::getWritableINodes(const char* path, const char* username, const char* groupname, ostream* errorStream) {
	// get user info from name (no need to free)
	struct passwd* userInfo = UnixWrappers::getUserInfo(username, errorStream);
	if (userInfo == nullptr) return nullptr;
	
	// get group info from name (no need to free)
	struct group* groupInfo = UnixWrappers::getGroupInfo(groupname, errorStream);
	if (groupInfo == nullptr) return nullptr;
	
	// check iNode existance
	if (UnixWrappers::getINodeStat(path, errorStream) == nullptr) return nullptr;
	
	// get absolute path
	unique_ptr<char> absolutePath(canonicalize_file_name(path));
	
	// run permission checking itself
	return getWritableINodes(absolutePath.get(), userInfo->pw_uid, groupInfo->gr_gid, errorStream);
}

unique_ptr<vector<string>> INodeSelector::getWritableINodes(const char* path, uid_t uid, gid_t gid, ostream* errorStream) {
	unique_ptr<vector<string>> result(new vector<string>());
	unique_ptr<struct stat> iNodeStat = UnixWrappers::getINodeStat(path, errorStream);
	if (iNodeStat == nullptr) return result;
	
	bool isFile = S_ISREG(iNodeStat->st_mode);
	bool isDir = S_ISDIR(iNodeStat->st_mode);
	// We ignore everything that is not a file or directory
	if (isFile == false && isDir == false) return result;
	
	if (PermissionChecker::isWritable(*iNodeStat, uid, gid)) {
		result->push_back(string(isFile ? "f " : "d ") + path);
	}
	
	// If we are allowed to read folder, read it
	if (isDir && PermissionChecker::isExecutable(*iNodeStat, uid, gid)) {
		unique_ptr<vector<string>> entries = Directory::getDirectoryEntries(path, errorStream);
		if (entries == nullptr) return result;
		
		for (string entry : *entries) {
			string newPath(path);
			if (newPath.back() != '/') newPath.append("/");
			newPath.append(entry);
			unique_ptr<vector<string>> newResult = getWritableINodes(newPath.c_str(), uid, gid, errorStream);
			result->insert(result->end(), newResult->begin(), newResult->end());
		}
	}

	return result;
}