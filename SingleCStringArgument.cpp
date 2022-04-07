#include "SingleCStringArgument.h"

using namespace std;

bool SingleCStringArgument::isValid() {
	if (_notNull && _value == nullptr && errorStream != nullptr) 
		*errorStream << "Error: argument '" << _argumentName << "' was not specified. Use "
		<< aliasList[0] << " <" << _argumentName << "> for that.\n";
	return _notNull == false || _value != nullptr;
}

bool SingleCStringArgument::handleArgument(int& index, int length, char** arguments) {
	if (_specificationsCount >= _maxSpecifications) {
		if (errorStream != nullptr)
			*errorStream << "Error: argument '" << _argumentName << "' was specified too many times\n";
		return false;
	}
	_specificationsCount++;
	
	if (++index >= length) {
		if (errorStream != nullptr)
			*errorStream << "Error: argument '" << _argumentName << "' is missing a value\n";
		return false;
	}
	_value = arguments[index];
	return true;
}

SingleCStringArgument::SingleCStringArgument(string name, initializer_list<string> aliases, int maxSpecifications, 
                        bool notNull, ostream* errorStream, void* defaultValue)
					  : CLIArgument(aliases, errorStream, defaultValue), _argumentName(name), 
						_maxSpecifications(maxSpecifications), _notNull(notNull) {};
