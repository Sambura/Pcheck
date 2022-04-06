//#include <string>

//using namespace std;

void permissionCheck(char* username, char* groupname, char* path);
bool isWritable(struct stat& iNode, int uid, int gid);
bool isExecutable(struct stat& iNode, int uid, int gid);
void isWritableRecursively(const char* path, int uid, int gid);