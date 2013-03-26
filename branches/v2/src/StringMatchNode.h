#ifndef _STRINGMATCHNODE_H_
#define _STRINGMATCHNODE_H_

#include <string>
#include <set>

#include "WildcardStringMatcher.h"

using namespace std;

class StringEntryNode;

class StringMatchNode
{
public:
	StringMatchNode(string initial, StringEntryNode * e);
	StringMatchNode(StringMatchNode& other);

	~StringMatchNode();

	StringMatchNode& operator= (const StringMatchNode& other);

	int FindOverlapIndex(string check);
	string FindOverlap(string check);

	void SplitAndMerge(string check, StringEntryNode * e);
	void Merge(string s, StringEntryNode * e);

	void Unlink(string nodename);

	StringEntryNode * Check(string needle);
	StringEntryNode * DoesMatch(string needle);

	StringMatchNode * PickOverlappingChild(string s);

	string GetIdentity();
	void DebugTraverseAll(string path, int depth);

	void Rebrand(string newid);
	void NormalizeChildrenOf(StringMatchNode * child);

	StringEntryNode * entry;
	WildcardStringMatcher::nodeset children;
	string identity;
	bool am_i_wild;

	static bool ReferenceCompare(StringMatchNode * a, StringMatchNode * b);
};

#endif
