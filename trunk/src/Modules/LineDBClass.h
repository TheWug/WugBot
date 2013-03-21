#ifndef _LINEDBCLASS_H_
#define _LINEDBCLASS_H_

#include <string>
#include <set>

using namespace std;

class Line
{
	public:
	string type;
	string author;
	string message;
	unsigned long long timestamp;
};

template <int I>
class LineDB
{
	public:
	LineDB();

	string owner;

	Line lines[I];
	int current;
	int size;

	void AddLine(string t, string s);
	void AddLine(string t, string s, string n);

	Line GetLine(int i);
	int GetIndexOfNextLineOfType(set<string> types, int first);
	int GetDeltaMillisBetweenMessages(int first, int second);

	int Size();

	static const int LIMIT = I;
};

#endif
