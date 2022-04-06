#pragma once
#include <ostream>
#include <iostream>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>

void permissionCheck(const char* username, const char* groupname, const char* path);
void isWritableRecursively(const char* path, int uid, int gid);
bool isSelfOrParent(const char* path);
struct passwd* getUserInfo(const char* username, std::ostream& errorStream = std::cerr);
struct group* getGroupInfo(const char* groupname, std::ostream& errorStream = std::cerr);
struct stat* getINodeStat(const char* path, std::ostream& errorStream = std::cerr);