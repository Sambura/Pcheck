#include "pcheck.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>
#include <iostream>
#include <cstring>

using namespace std;

void permissionCheck(char* username, char* groupname, char* path) {
	// get user info from name
	errno = 0;
	struct passwd* userInfo = getpwnam(username);
	if (userInfo == nullptr) {
		switch (errno) {
			case 0:
			case ENOENT:
			case ESRCH:
			case EBADF:
			case EPERM:
				cout << "The given username was not found\n";
				return;
			default:
				perror("Problem with username: ");
				return;
		}
	}
	
	// get group info from name
	errno = 0;
	struct group* groupInfo = getgrnam(groupname);
	if (groupInfo == nullptr) {
		switch (errno) {
			case 0:
			case ENOENT:
			case ESRCH:
			case EBADF:
			case EPERM:
				cout << "The given groupname was not found\n";
				return;
			default:
				perror("Problem with groupname: ");
				return;
		}
	}
	
	// get inode information
	struct stat iNodeStat;
	int pathLength = strlen(path);
	if (path[pathLength - 1] == '/') path[pathLength - 1] = '\0';
	if (stat(path, &iNodeStat) < 0) {
		switch (errno) {
			case EACCES:
				cout << "Some part of path is not accessible by you. Try running from sudo!\n"; 
				return;
			case ENOENT:
			case ENOTDIR:
				cout << "Specified path does not exists or is invalid\n";
				return;
			default:
				perror("Problem with path: ");
				return;
		}
	}
	
	int userID = userInfo->pw_uid;
	int groupID = groupInfo->gr_gid;
	
	isWritableRecursively(path, userID, groupID);
}

void isWritableRecursively(const char* path, int uid, int gid) {
	struct stat iNodeStat;
	if (lstat(path, &iNodeStat) < 0) {
		perror((string("Warning: could not read iNode: ") + string(path) + string(": ")).c_str());
		return;
	}
	//bool isLink = S_ISLNK(iNodeStat.st_mode);
	bool isFile = S_ISREG(iNodeStat.st_mode);
	bool isDir = S_ISDIR(iNodeStat.st_mode);
	if (isFile == false && isDir == false) return;
	if (isWritable(iNodeStat, uid, gid)) {
		cout << (isFile ? "f" : "d") << " " << path << '\n';
	}
	
	// If we are allowed to read folder, read it
	if (isDir && isExecutable(iNodeStat, uid, gid)) {
		DIR* dir;
		struct dirent* ent;
		if ((dir = opendir(path)) != nullptr) {
			while ((ent = readdir(dir)) != nullptr) {
				if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
					continue;
				string newPath(path);
				newPath.append("/");
				newPath.append(ent->d_name);
				isWritableRecursively(newPath.c_str(), uid, gid);
			}
			closedir(dir);
		} 
		else
			perror((string("Warning: could not read directory ") + string(path) + string(": ")).c_str());
	}
}

bool isWritable(struct stat& iNode, int uid, int gid) {
	if (uid == 0) return true; // if the user is root

	// if the specified user is the owner of inode
	if (iNode.st_uid == uid) return iNode.st_mode & S_IWUSR;
	
	// if the specified group is owner group
	if (iNode.st_gid == gid) return iNode.st_mode & S_IWGRP;
	
	// otherwise
	return iNode.st_mode & S_IWOTH;
}

bool isExecutable(struct stat& iNode, int uid, int gid) {
	if (uid == 0) return true; // if the user is root

	// if the specified user is the owner of inode
	if (iNode.st_uid == uid) return iNode.st_mode & S_IXUSR;
	
	// if the specified group is owner group
	if (iNode.st_gid == gid) return iNode.st_mode & S_IXGRP;
	
	// otherwise
	return iNode.st_mode & S_IXOTH;
}