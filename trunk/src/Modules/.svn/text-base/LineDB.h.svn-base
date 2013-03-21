#ifndef _LINEDB_H_
#define _LINEDB_H_

#include <iostream>
#include "Bottime.h"
#include "LineDBClass.h"

using namespace std;


template <int I> LineDB<I>::LineDB()
{
	size = 0;
	current = 0;
}

template <int I> Line LineDB<I>::GetLine(int i)
{
	if (i >= size || i < -size)
		return Line();
	return lines[(current - i + LIMIT) % LIMIT];
}

template <int I> void LineDB<I>::AddLine(string t, string s)
{
	current = (current + 1) % LIMIT;
	++size;
	lines[current].message = s;
	lines[current].type = t;
	lines[current].author = "";
	lines[current].timestamp = Bottime::GetCurrentTimeMillis();
	if (size > LIMIT)
		size = LIMIT;
}

template <int I> void LineDB<I>::AddLine(string t, string s, string n)
{
	current = (current + 1) % LIMIT;
	++size;
	lines[current].message = s;
	lines[current].type = t;
	lines[current].author = n;
	lines[current].timestamp = Bottime::GetCurrentTimeMillis();
	if (size > LIMIT)
		size = LIMIT;
}

template <int I> int LineDB<I>::Size()
{
	return size;
}

template <int I> int LineDB<I>::GetIndexOfNextLineOfType(set<string> types, int first)
{
	for (int i = first; i < size; ++i)
	{
		if (types.find(GetLine(i).type) != types.end())
			return i;
	}
	return -1;
}

#endif
