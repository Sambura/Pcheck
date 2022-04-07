#include "UnixWrappers.h"
#include <cstring>
 
using namespace std;

struct passwd* UnixWrappers::getUserInfo(const char* username, ostream* errorStream) {
	errno = 0;
	struct passwd* userInfo = getpwnam(username);
	
	if (userInfo == nullptr) {
		if (errorStream == nullptr) return nullptr;
		switch (errno) {
			case 0:
			case ENOENT:
			case ESRCH:
			case EBADF:
			case EPERM:
				*errorStream << "The given username was not found\n";
				return nullptr;
			default:
				*errorStream << "Problem with username: " << strerror(errno) << '\n';
				return nullptr;
		}
	}
	
	return userInfo;
}

struct group* UnixWrappers::getGroupInfo(const char* groupname, ostream* errorStream) {
	errno = 0;
	struct group* groupInfo = getgrnam(groupname);
	
	if (groupInfo == nullptr) {
		if (errorStream == nullptr) return nullptr;
		switch (errno) {
			case 0:
			case ENOENT:
			case ESRCH:
			case EBADF:
			case EPERM:
				*errorStream << "The given groupname was not found\n";
				return nullptr;
			default:
				*errorStream << "Problem with groupname: " << strerror(errno) << '\n';
				return nullptr;
		}
	}
	
	return groupInfo;
}

unique_ptr<struct stat> UnixWrappers::getINodeStat(const char* path, ostream* errorStream) {
	errno = 0;
	unique_ptr<struct stat> iNodeStat(new struct stat());
	
	if (stat(path, iNodeStat.get()) < 0) {
		if (errorStream == nullptr) return nullptr;
		switch (errno) {
			case EACCES:
				*errorStream << "Cannot access specified path: permission denied\n"; 
				return nullptr;
			case ENOENT:
			case ENOTDIR:
				*errorStream << "Specified path does not exists or is invalid\n";
				return nullptr;
			default:
				*errorStream << "Problem with path: " << strerror(errno) << '\n';
				return nullptr;
		}
	}
	
	return iNodeStat;
}
