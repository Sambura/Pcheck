#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <initializer_list>
#include <vector>

using namespace std;

struct CLIArgument {
public:
	void* value = nullptr;
	vector<string> aliasList;
	function<bool(const CLIArgument&)> verifier;
	
	CLIArgument(function<bool(const CLIArgument&)> verifier,
				initializer_list<string> aliases)
		: verifier(verifier), aliasList(aliases) {}
};

int main(int argc, char* argv[]) {
	vector<CLIArgument*> cliArguments = {
		new CLIArgument(
			[](const CLIArgument& argument) -> bool { 
				if (argument.value != nullptr) return true;
				cout << "Error: parameter 'username' was not specified. Use '-u <username>' for that\n";
				return false;
			}, {"-u", "--username"}
		),
		new CLIArgument(
			[](const CLIArgument& argument) -> bool { 
				if (argument.value != nullptr) return true;
				cout << "Error: parameter 'group name' was not specified. Use '-g <groupname>' for that\n";
				return false;
			}, {"-g", "--groupname"}),
		new CLIArgument(
			[](const CLIArgument& argument) -> bool { 
				if (argument.value != nullptr) return true;
				cout << "Error: parameter 'path' was not specified. Use '-p <path>' for that\n";
				return false;
			}, {"-p", "--path"}),
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
		string* argString = new string(argv[i]);
		// check, whether we have such argument
		if (argumentsMap.find(*argString) == argumentsMap.end()) {
			cout << "Unknown argument: " << *argString << '\n';
			return -1;
		}
		
		CLIArgument* argument = argumentsMap[*argString];
		if (argument->value != nullptr) {
			cout << "Error: the argument " << *argString << " was specified more than once\n";
			return -1;
		}
		
		if (++i >= argc) {
			cout << "Error: the argument " << *argString << " is missing a value\n";
			return -1;
		}
		
		argument->value = argString;
	}
	
	// Check the correctness of arguments
	for (CLIArgument* argument : cliArguments) {
		if (argument->verifier(*argument) == false) return -1;
	}
	
	// run the tool
	return 0;
}
