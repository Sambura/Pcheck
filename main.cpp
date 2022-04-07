#include "INodeSelector.h"
#include "SingleCStringArgument.h"
#include <unordered_map>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
	vector<CLIArgument*> cliArguments = {
		new SingleCStringArgument("username", {"-u", "--username"}, 1, true, &cerr),
		new SingleCStringArgument("groupname", {"-g", "--groupname"}, 1, true, &cerr),
		new SingleCStringArgument("path", {"-p", "--path"}, 1, true, &cerr),
	};
	
	// build argument map
	unordered_map<string, CLIArgument*> argumentsMap;
	for (CLIArgument* argument : cliArguments) {
		for (string alias : argument->aliasList) {
			argumentsMap[alias] = argument;
		}
	}
	
	// read all CLI arguments and store their values
	for (int i = 1; i < argc; i++) {
		string argString(argv[i]);
		// check, whether we have such argument
		if (argumentsMap.find(argString) == argumentsMap.end()) {
			cout << "Unknown argument: " << argString << '\n';
			return -1;
		}
		
		if (argumentsMap[argString]->handleArgument(i, argc, argv) == false) return -1;
	}
	
	// Check the correctness of arguments
	for (CLIArgument* argument : cliArguments) {
		if (argument->isValid() == false) return -1;
	}
	
	auto result = INodeSelector::getWritableINodes((char*)argumentsMap["--path"]->getValue(),
													(char*)argumentsMap["--username"]->getValue(),
													(char*)argumentsMap["--groupname"]->getValue(),
													&cerr);

	if (result == nullptr) return -1;

	for (string path : *result) {
		cout << path << '\n';
	}

	return 0;
}
