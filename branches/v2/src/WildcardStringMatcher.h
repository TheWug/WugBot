#ifndef _WILDCARDSTRINGMATCHER_H_
#define _WILDCARDSTRINGMATCHER_H_

#include <set>
#include <string>

using namespace std;

class StringMatchNode;
class StringEntryNode;

class WildcardStringMatcher
{
public:
	typedef set<StringMatchNode *, bool (*)(StringMatchNode *, StringMatchNode *)> nodeset;

private:
	nodeset children;

	StringMatchNode * PickOverlappingChild(string s);
	void NormalizeChildrenOf(StringMatchNode * child);
	StringEntryNode * InnerMatch(string needle);
public:
	WildcardStringMatcher();
	WildcardStringMatcher(const WildcardStringMatcher& other);

	~WildcardStringMatcher();
	WildcardStringMatcher& operator= (const WildcardStringMatcher& other);

	void Add(string newstring);		// done.  aww yeah
	void Remove(string oldstring);		// done.  aww yeah

	bool Check(string needle);		// done.  aww yeah
	string Match(string needle);		// done.  aww yeah
	// set<string> AllMatches(string needle); // later

	void DebugTraverseAll();
};

#endif
