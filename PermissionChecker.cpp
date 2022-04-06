#include "PermissionChecker.h"

bool PermissionChecker::isReadable(const struct stat& iNode, uid_t uid, gid_t gid) {
	if (uid == 0) return true; // if the user is root

	// if the specified user is the owner of inode
	if (iNode.st_uid == uid) return iNode.st_mode & S_IRUSR;
	
	// if the specified group is owner group
	if (iNode.st_gid == gid) return iNode.st_mode & S_IRGRP;
	
	// otherwise
	return iNode.st_mode & S_IROTH;
}

bool PermissionChecker::isWritable(const struct stat& iNode, uid_t uid, gid_t gid) {
	if (uid == 0) return true; // if the user is root

	// if the specified user is the owner of inode
	if (iNode.st_uid == uid) return iNode.st_mode & S_IWUSR;
	
	// if the specified group is owner group
	if (iNode.st_gid == gid) return iNode.st_mode & S_IWGRP;
	
	// otherwise
	return iNode.st_mode & S_IWOTH;
}

bool PermissionChecker::isExecutable(const struct stat& iNode, uid_t uid, gid_t gid) {
	if (uid == 0) return true; // if the user is root

	// if the specified user is the owner of inode
	if (iNode.st_uid == uid) return iNode.st_mode & S_IXUSR;
	
	// if the specified group is owner group
	if (iNode.st_gid == gid) return iNode.st_mode & S_IXGRP;
	
	// otherwise
	return iNode.st_mode & S_IXOTH;
}