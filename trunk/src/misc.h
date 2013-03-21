#ifndef _MISC_H_
#define _MISC_H_

#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <map>

using namespace std;

vector<string> splitcommands(const string& s, const string& delim, const bool keep_empty = true);
vector<string> splitandterminate(const string& s);

string tolower(string str);
string toupper(string str);

string tostr(int num);
string tostr(float num);

string AsHumanReadableTime(unsigned long long millis);

string trim(string);

string getline(istringstream& stream);

string stringreplace(string haystack, string needle, string replacewith);

bool IsAlphaNumeric(char c);
bool IsNumeric(char c);

string ToAlphaNumeric(string s);
bool IsNumeric(string s);

bool IsValidNick(string nick);

string GetIP(unsigned int ip);

inline int mod(int x, int m)
{
    int r = x%m;
    return r<0 ? r+m : r;
}

string ToString(set<string> s);

string ToString(map<string, string> s);

string ToString(map<string, set<string> > s);

template <typename T> void vector_purge(vector<T>& v, const T& t)
{
	int readp = 0, writep = 0, size = v.size();

	while (readp < size)
	{
		if (v.at(readp) == t)
		{
			++readp;
			continue;
		}
		if (readp != writep)
			v.at(writep) = v.at(readp);
		++readp; ++writep;
	}
	v.resize(writep);
}

#endif
