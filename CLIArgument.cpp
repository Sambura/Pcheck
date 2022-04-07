#include "CLIArgument.h"

using namespace std;

void* CLIArgument::getValue() { return _value; }
CLIArgument::CLIArgument(initializer_list<string> aliases, ostream* errorStream, void* defaultValue)
						 : aliasList(aliases), errorStream(errorStream), _value(defaultValue) {}