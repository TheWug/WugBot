#include "WildcardStringMatcher.h"

#include <iostream>

#include "StringMatchNode.h"
#include "StringEntryNode.h"

WildcardStringMatcher::WildcardStringMatcher() : children(StringMatchNode::ReferenceCompare)
{
	return;
}

WildcardStringMatcher::WildcardStringMatcher(const WildcardStringMatcher& other) : children(StringMatchNode::ReferenceCompare)
{
	*this = other;
}

WildcardStringMatcher& WildcardStringMatcher::operator= (const WildcardStringMatcher& other)
{
	if (this == &other) return *this; // same object
	for (nodeset::iterator i = other.children.begin(); i != other.children.end(); ++i) children.insert(new StringMatchNode(**i));
	return *this;
}

WildcardStringMatcher::~WildcardStringMatcher()
{
	for (nodeset::iterator i = children.begin(); i != children.end(); ++i) delete *i;
	return;
}

void WildcardStringMatcher::Add(string s)
{
	if (s == "") return;
	StringEntryNode * e = new StringEntryNode(s);
	StringMatchNode * babby = PickOverlappingChild(s);
	if (babby == NULL) children.insert(new StringMatchNode(s, e));
	else if (babby->GetIdentity() == s)
	{
		delete babby->entry;
		babby->entry = e;
	}
	else if (s.find(babby->GetIdentity()) == 0) babby->Merge(s, e);
	else babby->SplitAndMerge(s, e);
	return;
}

void WildcardStringMatcher::Remove(string s)
{
	StringMatchNode * child = PickOverlappingChild(s);
	if (child == NULL || s.find(child->GetIdentity()) != 0) return;
	child->Unlink(s.substr(child->GetIdentity().length()));
	NormalizeChildrenOf(child);
	return;
}

bool WildcardStringMatcher::Check(string needle)
{
	return (InnerMatch(needle) != NULL);
}

string WildcardStringMatcher::Match(string needle)
{
	StringEntryNode * entry = InnerMatch(needle);
	if (entry != NULL) return entry->GetValue();
	return "";
}

StringEntryNode * WildcardStringMatcher::InnerMatch(string needle)
{
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

StringMatchNode * WildcardStringMatcher::PickOverlappingChild(string s)
{
	StringMatchNode node(s, NULL);
	nodeset::iterator i = children.lower_bound(&node);
	if (i != children.end() && (*i)->FindOverlapIndex(s)) return *i;
	if (i != children.begin() && (*--i)->FindOverlapIndex(s)) return *i;
	return NULL;
}

void WildcardStringMatcher::DebugTraverseAll()
{
	for (nodeset::iterator i = children.begin(); i != children.end(); ++i)
	{
		(*i)->DebugTraverseAll((*i)->GetIdentity(), 0);
	}
	return;
}

void WildcardStringMatcher::NormalizeChildrenOf(StringMatchNode * child)
{
	if (child->children.size() > 1 || child->entry != NULL) return;
	if (child->children.size() == 1)
	{
		StringMatchNode * grandchild = *child->children.begin();
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
}
