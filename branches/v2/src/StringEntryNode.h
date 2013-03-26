#ifndef _STRINGENTRYNODE_H_
#define _STRINGENTRYNODE_H_

#include <string>

using namespace std;

class StringEntryNode
{
private:
	string value;

public:
	StringEntryNode(string newval);
	~StringEntryNode();

	string GetValue();
};

#endif
