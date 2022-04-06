#include "pcheck.h"
#include "PermissionChecker.h"
#include <unistd.h>
#include <dirent.h>
#include <cstring>
#include <stdlib.h>

using namespace std;

// prins absolute paths of all files and directories to, which can be written to
// by the process with the given uid and gid. 
void permissionCheck(const char* username, const char* groupname, const char* path) {
	// get user info from name (no need to free)
	struct passwd* userInfo = getUserInfo(username);
	if (userInfo == nullptr) return;
	
	// get group info from name (no need to free)
	struct group* groupInfo = getGroupInfo(groupname);
	if (groupInfo == nullptr) return;
	
	// check iNode existance
	struct stat* iNodeStat = getINodeStat(path);
	if (iNodeStat == nullptr) return;
	free(iNodeStat);
	
	// get absolute path
	char* absolutePath = canonicalize_file_name(path);
	
	// run permission checking itself
	isWritableRecursively(absolutePath, userInfo->pw_uid, groupInfo->gr_gid);
	
	// free allocated memory
	free(absolutePath);
}

struct passwd* getUserInfo(const char* username, ostream& errorStream) {
	errno = 0;
	struct passwd* userInfo = getpwnam(username);
	
	if (userInfo == nullptr) {
		switch (errno) {
			case 0:
			case ENOENT:
			case ESRCH:
			case EBADF:
			case EPERM:
				errorStream << "The given username was not found\n";
				return nullptr;
			default:
				errorStream << "Problem with username: " << strerror(errno) << '\n';
				return nullptr;
		}
	}
	
	return userInfo;
}

struct group* getGroupInfo(const char* groupname, ostream& errorStream) {
	errno = 0;
	struct group* groupInfo = getgrnam(groupname);
	
	if (groupInfo == nullptr) {
		switch (errno) {
			case 0:
			case ENOENT:
			case ESRCH:
			case EBADF:
			case EPERM:
				errorStream << "The given groupname was not found\n";
				return nullptr;
			default:
				errorStream << "Problem with groupname: " << strerror(errno) << '\n';
				return nullptr;
		}
	}
	
	return groupInfo;
}

struct stat* getINodeStat(const char* path, ostream& errorStream) {
	errno = 0;
	struct stat* iNodeStat = new struct stat();
	
	if (stat(path, iNodeStat) < 0) {
		switch (errno) {
			case EACCES:
				errorStream << "Some part of path is not accessible by you. Try running from sudo!\n"; 
				return nullptr;
			case ENOENT:
			case ENOTDIR:
				errorStream << "Specified path does not exists or is invalid\n";
				return nullptr;
			default:
				errorStream << "Problem with path: " << strerror(errno) << '\n';
				return nullptr;
		}
	}
	
	return iNodeStat;
}

void isWritableRecursively(const char* path, int uid, int gid) {
	struct stat iNodeStat;
	if (lstat(path, &iNodeStat) < 0) {
		cerr << "Warning: could not stat iNode " << path << ": " << strerror(errno) << '\n';
		return;
	}
	
	bool isFile = S_ISREG(iNodeStat.st_mode);
	bool isDir = S_ISDIR(iNodeStat.st_mode);
	if (isFile == false && isDir == false) return;
	
	if (PermissionChecker::isWritable(iNodeStat, uid, gid)) {
		cout << (isFile ? "f" : "d") << " " << path << '\n';
	}
	
	// If we are allowed to read folder, read it
	if (isDir && PermissionChecker::isExecutable(iNodeStat, uid, gid)) {
		DIR* dir = opendir(path);
		if (dir == nullptr) {
			cerr << "Warning: could not read directory " << path << ": " << strerror(errno) << '\n';
			return;
		}

		struct dirent* ent;
		
		while ((ent = readdir(dir)) != nullptr) {
			if (isSelfOrParent(ent->d_name)) continue;
			
			string newPath(path);
			if (newPath.back() != '/') newPath.append("/");
			newPath.append(ent->d_name);
			isWritableRecursively(newPath.c_str(), uid, gid);
		}
		
		closedir(dir);	
	}
}

// is directory entry equals to '.' or '..' ?
bool isSelfOrParent(const char* path) {
	return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}
