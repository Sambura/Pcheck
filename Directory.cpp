#include "Directory.h"
#include <cstring>

using namespace std;

unique_ptr<vector<string>> Directory::getDirectoryEntries(const char* path, ostream* errorStream) {
	DIR* dir = opendir(path);
	if (dir == nullptr) {
		*errorStream << "Could not read directory " << path << ": " << strerror(errno) << '\n';
		return nullptr;
	}

	struct dirent* ent;
	unique_ptr<vector<string>> entries(new vector<string>());
	
	while ((ent = readdir(dir)) != nullptr) {
		if (isSelfOrParent(ent->d_name)) continue;
		
		entries->push_back(ent->d_name);
	}
	
	closedir(dir);

	return entries;
}

bool Directory::isSelfOrParent(const char* path) {
	return strcmp(path, ".") == 0 || strcmp(path, "..") == 0;
}
