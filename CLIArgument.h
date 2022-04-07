#pragma once
#include <ostream>
#include <vector>
#include <string>
#include <initializer_list>

// Class that represents a single command line argument
class CLIArgument {
protected:
	void* _value = nullptr;

public:
	// ostream where error messages should be printed
	std::ostream* errorStream;
	// vector of argument aliases
	std::vector<std::string> aliasList;

    // Returns a pointer to a value stored in this argument
	void* getValue();
    // Tells, whether the current value of argument is valid or not
	bool virtual isValid() = 0;
    // Reads argument array from given position, extracting argument value from it
	// Returns true on success, false on error
	bool virtual handleArgument(int& index, int length, char** arguments) = 0;

	CLIArgument(std::initializer_list<std::string> aliases, std::ostream* errorStream, void* defaultValue = nullptr);
};