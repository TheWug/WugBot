#include "StringEntryNode.h"

#include <iostream>

StringEntryNode::StringEntryNode(string newval)
{
	value = newval;
	return;
}

StringEntryNode::~StringEntryNode()
{
}

string StringEntryNode::GetValue()
{
	return value;
}
