#pragma once
#include "CLIArgument.h"

class SingleCStringArgument : public CLIArgument {
protected:
	std::string _argumentName;
	int _maxSpecifications;
	int _specificationsCount;
	bool _notNull;

public:
    // Tells, whether the current value of argument is valid or not
    // The argument is valid, if the pointer to value is not nullptr, or if 'notNull'
    // field is set to false (value is set through the constructor)
	bool virtual isValid();
    // Reads argument array from given position, extracting argument value from it
    // The value pointer gets assigned with the arguments[index + 1] string.
    // Returns true on success, false on error
	bool virtual handleArgument(int& index, int length, char** arguments);

	SingleCStringArgument(std::string name, std::initializer_list<std::string> aliases, int maxSpecifications = 1, bool notNull = true, 
						  std::ostream* errorStream = nullptr, void* defaultValue = nullptr);

};