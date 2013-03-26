#include "StringMatchNode.h"

#include <algorithm>
#include <iostream>
#include <iomanip>

#include "StringEntryNode.h"

StringMatchNode::StringMatchNode(string initial, StringEntryNode * e) : children(StringMatchNode::ReferenceCompare)
{
	int firstwild;

	{
		firstwild = initial.find('*');
		int firstqmrk = initial.find('?');
		int firsthash = initial.find('#');

		if (firstwild == string::npos || firstqmrk != string::npos && firstqmrk < firstwild) firstwild = firstqmrk;
		if (firstwild == string::npos || firsthash != string::npos && firsthash < firstwild) firstwild = firsthash;
	}

	string chaininitial;
	if (firstwild == 0)
	{
		am_i_wild = true;
		identity = initial.substr(0, 1);
		chaininitial = initial.substr(1);
	}
	else if (firstwild != string::npos)
	{
		am_i_wild = false;
		identity = initial.substr(0, firstwild);
		chaininitial = initial.substr(firstwild);
	}
	else
	{
		am_i_wild = false;
		identity = initial;
		chaininitial = "";
	}

	if (chaininitial == "") entry = e;
	else
	{
		entry = NULL;
		children.insert(new StringMatchNode(chaininitial, e));
	}
	
	return;
}

StringMatchNode::StringMatchNode(StringMatchNode& other) : children(StringMatchNode::ReferenceCompare)
{
	*this = other;
}

StringMatchNode::~StringMatchNode()
{
	if (entry != NULL) delete entry;
	for (WildcardStringMatcher::nodeset::iterator i = children.begin(); i != children.end(); ++i) delete *i;
	return;
}

StringMatchNode& StringMatchNode::operator= (const StringMatchNode& other)
{
	if (this == &other) return *this;
	am_i_wild = other.am_i_wild;
	if (other.entry != NULL) entry = new StringEntryNode(*other.entry);
	else entry = NULL;
	identity = other.identity;
	for (WildcardStringMatcher::nodeset::iterator i = other.children.begin(); i != other.children.end(); ++i) children.insert(new StringMatchNode(**i));
	return *this;
}

void StringMatchNode::Rebrand(string newid)
{
	identity = newid;
}

int StringMatchNode::FindOverlapIndex(string check)
{
	int i = 0, maximum = min(check.length(), identity.length());
	while (check[i] == identity[i] && i < maximum) ++i;
	return i;
}

string StringMatchNode::FindOverlap(string check)
{
	int length = FindOverlapIndex(check);
	return identity.substr(0, length);
}

void StringMatchNode::SplitAndMerge(string check, StringEntryNode * e)
{
	int split_pos = FindOverlapIndex(check);
	string first = identity.substr(0, split_pos);
	string second = identity.substr(split_pos, identity.length() - split_pos);
	check = check.substr(split_pos, check.length() - split_pos);

	StringMatchNode * newnode = new StringMatchNode(second, entry);
	StringMatchNode * newchild = new StringMatchNode(check, e);

	newnode->children = children;
	children.clear();
	children.insert(newnode);
	children.insert(newchild);

	Rebrand(first);
	entry = NULL;
	return;
}

bool StringMatchNode::ReferenceCompare(StringMatchNode * a, StringMatchNode * b)
{
	return a->identity < b->identity;
}

StringMatchNode * StringMatchNode::PickOverlappingChild(string s)
{
	StringMatchNode node(s, NULL);
	WildcardStringMatcher::nodeset::iterator i = children.lower_bound(&node);
	if (i != children.end() && (*i)->FindOverlapIndex(s)) return *i;
	else if (i != children.begin() && (*--i)->FindOverlapIndex(s)) return *i;
	return NULL;
}

void StringMatchNode::Merge(string s, StringEntryNode * e)
{
	int index = FindOverlapIndex(s);
	s = s.substr(index, s.length() - index);
	StringMatchNode * babby = PickOverlappingChild(s);
	if (babby == NULL) children.insert(new StringMatchNode(s, e));
	else if (babby->identity == s)
	{
		delete babby->entry;
		babby->entry = e;
	}
	else if (s.find(babby->identity) == 0) babby->Merge(s, e);
	else babby->SplitAndMerge(s, e);
}

string StringMatchNode::GetIdentity()
{
	return identity;
}

void StringMatchNode::DebugTraverseAll(string path, int depth)
{
	cout << setw(depth * 4) << " " << identity << ": ";

	if (am_i_wild) cout << "(wild) ";

	if (entry != NULL) cout << entry->GetValue() << (entry->GetValue() == path ? " (matches)" : " (DOES NOT MATCH)") << endl;
	else cout << "no entry" << endl;
	
	for (WildcardStringMatcher::nodeset::iterator i = children.begin(); i != children.end(); ++i)
	{
		(*i)->DebugTraverseAll(path + (*i)->GetIdentity(), depth + 1);
	}
}

void StringMatchNode::Unlink(string nodename)
{
	if (nodename == "" && entry != NULL)
	{
		delete entry;
		entry = NULL;
	}
	else
	{
		StringMatchNode * child = PickOverlappingChild(nodename);
		if (child == NULL || nodename.find(child->GetIdentity()) != 0) return;
		child->Unlink(nodename.substr(child->GetIdentity().length()));
		NormalizeChildrenOf(child);
	}
	return;
}

void StringMatchNode::NormalizeChildrenOf(StringMatchNode * child)
{
	if (child->children.size() > 1 || child->entry != NULL);
	else if (child->children.size() == 1)
	{
		StringMatchNode * grandchild = *child->children.begin();
		if (child->am_i_wild || grandchild->am_i_wild) return;
		string childname = child->identity;
		child->children.erase(grandchild);
		children.erase(child);
		delete child;
		children.insert(grandchild);
		grandchild->Rebrand(childname + grandchild->identity);
	}
	else if (child->children.size() == 0)
	{
		children.erase(child);
		delete child;
	}
	return;
}

StringEntryNode * StringMatchNode::Check(string needle)
{
	if (needle == "" && entry != NULL) return entry;
	StringMatchNode * child = PickOverlappingChild(needle);
	StringEntryNode * output;
	if (child != NULL && (output = child->DoesMatch(needle)) != NULL) return output;
	child = PickOverlappingChild("*");
	if (child != NULL && (output = child->DoesMatch(needle)) != NULL) return output;
	child = PickOverlappingChild("#");
	if (child != NULL && (output = child->DoesMatch(needle)) != NULL) return output;
	child = PickOverlappingChild("?");
	if (child != NULL && (output = child->DoesMatch(needle)) != NULL) return output;
	return NULL;
}

StringEntryNode * StringMatchNode::DoesMatch(string needle)
{
	StringMatchNode * babby;
	StringEntryNode * temp;
	if (am_i_wild)
	{
		if (identity == "*")
		{
			for (int i = 0; i <= needle.length(); ++i)
			{
				if ((temp = Check(needle.substr(i))) != NULL) return temp;
			}
		}
		else if (identity == "#")
		{
			int i;
			for (i = 0; i < needle.length(); ++i)
			{
				char d = needle[i];
				if (d < '0' || d > '9') break;
			}
			if (i == 0) return NULL;
			return Check(needle.substr(i));
		}
		else if (identity == "?")
		{
			if (needle.length() < 1) return NULL;
			return Check(needle.substr(1));
		}
		return false;
	}
	else if (needle.find(identity) == 0)
	{
		return Check(needle.substr(identity.length()));
	}
	return NULL;
}
