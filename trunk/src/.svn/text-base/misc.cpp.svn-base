#include <iostream>
#include <algorithm>
#include <string> 
#include <vector>
#include <sstream>

#include "misc.h"

using std::string;
using std::vector;

bool IsNumeric(string s)
{
	for (int i = 0; i < s.length(); ++i)
	{
		if (!IsNumeric(s[i])) return false;
	}
	return true;
}

vector<string> splitcommands(const string& s, const string& delim, const bool keep_empty)
{
	vector<string> result;
	if (delim.empty()) {
		result.push_back(s);
		return result;
	}
	string::const_iterator substart = s.begin(), subend;
	while (true)
	{
		subend = search(substart, s.end(), delim.begin(), delim.end());
			string temp(substart, subend);
		if (keep_empty || !temp.empty())
			result.push_back(temp);
		if (subend == s.end())
			break;
		substart = subend + delim.size();
	}
	return result;
}

vector<string> splitandterminate(const string& s)
{
	vector<string> result;
	char c;
	int p = 0, l = 0;
	char * str = new char[s.length() + 1];
	str[s.length()] = 0;
	for(; p < s.length(); p++, l++)
	{
		c = s.at(p);
		if (c == '\r' || c == '\n')
		{
			str[l] = 0;
			if (l != 0)
				result.push_back(str);
			l = -1;
		}
		else
			str[l] = c;
	}
	str[l] = 0;
	if (l != 0)
		result.push_back(str);
	delete[] str;
	if (result.size() == 0)
		result.push_back("");
	return result;
}

string tolower(string str)
{
	string newstr(str.c_str());
	std::transform(newstr.begin(), newstr.end(), newstr.begin(),  (int (*)(int)) tolower);
	return newstr;
}

string toupper(string str)
{
	string newstr(str.c_str());
	std::transform(newstr.begin(), newstr.end(), newstr.begin(),  (int (*)(int)) toupper);
	return newstr;
}

string trim(string input)
{
	int begin = 0;
	int end = input.length() - 1;
	while (begin != input.length() && (input.at(begin) == ' ' || input.at(begin) == '\r' || input.at(begin) == '\n' || input.at(begin) == '\t'))
		begin++;
	if (begin == input.length())
		return "";
	while (end != -1 && (input.at(end) == ' ' || input.at(end) == '\r' || input.at(end) == '\n' || input.at(end) == '\t'))
		end--;
	return input.substr(begin, end - begin + 1);
}

string getline(istringstream& stream)
{
	static char datablock[32768];
	stream.getline(datablock, 32767);
	int i = 0;
	while(*(datablock + i) != 0 && (*(datablock + i) == ' ' || *(datablock + i) == '\t'))
		i++;
	return string(datablock + i); 
}

string tostr(int num)
{
	ostringstream temp;
	temp << num;
	return temp.str();
}

string stringreplace(string haystack, string needle, string replacewith)
{
	if (needle == "")
		return haystack;
	int begin = 0;
	int nlength = needle.length();
	int end;
	string out = "";
	while (true)
	{
		end = haystack.find(needle, begin);
		out.append(haystack.substr(begin, end - begin));
		if (end == -1)
			break;
		out.append(replacewith);
		begin = end + nlength;
	}
	return out;
}
		
bool IsAlphaNumeric(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9');
}

bool IsNumeric(char c)
{
	return (c >= '0' && c <= '9');
}

string ToAlphaNumeric(string s)
{
	string result = "";
	for (int i = 0; i < s.length(); ++i)
	{
		if (IsAlphaNumeric(s.at(i))) result.append(1, s.at(i));
		else if (s.at(i) == ' ' || s.at(i) == '\t') result.append(1, ' ');
	}
	return result;
}

bool IsValidNick(string nick)
{
	return true;
}

string GetIP(unsigned int ip)
{
	unsigned int lowest = ip % 256;
	unsigned int low = (ip / 256) % 256;
	unsigned int high = (ip / 65536) % 256;
	unsigned int highest = (ip / 16777216) % 256;
	return tostr((int)highest) + "." + tostr((int)high) + "." + tostr((int)low) + "." + tostr((int)lowest);
}

string tostr(float f)
{
	ostringstream temp;
	temp << f;
	return temp.str();	
}

string AsHumanReadableTime(unsigned long long millis)
{
	int seconds, minutes, hours, days, years;
	millis /= 1000;
	seconds = millis % 60;
	millis /= 60;
	minutes = millis % 60;
	millis /= 60;
	hours = millis % 24;
	millis /= 24;
	days = millis % 365;
	years = millis / 365;
	string result = "";
	if (years != 0)
	{
		result += tostr(years) + " year";
		if (years != 1) result += 's';
	}
	if (days != 0)
	{
		if (result != "") result += ", ";
		result += tostr(days) + " day";
		if (days != 1) result += 's';
	}
	if (hours != 0)
	{
		if (result != "") result += ", ";
		result += tostr(hours) + " hour";
		if (hours != 1) result += 's';
	}
	if (minutes != 0)
	{
		if (result != "") result += ", ";
		result += tostr(minutes) + " minute";
		if (minutes != 1) result += 's';
	}
	if (seconds != 0 || result == "")
	{
		if (result != "") result += ", ";
		result += tostr(seconds) + " second";
		if (seconds != 1) result += 's';
	}
	return result;
}

string ToString(set<string> s)
{
	set<string>::iterator i = s.begin();
	if (i == s.end())
		return "[]";
	string r = "[" + *i;
	++i;
	while (i != s.end())
	{
		r += string(" ") + *i;
		++i;
	}
	return r + "]";
}

string ToString(map<string, string> s)
{
	map<string, string>::iterator i = s.begin();
	if (i == s.end())
		return "[]";
	string r = "[" + i->first + ":" + i->second;
	++i;
	while (i != s.end())
	{
		r += string(" ") + i->first + ":" + i->second;
		++i;
	}
	return r + "]";
}

string ToString(map<string, set<string> > s)
{
	map<string, set<string> >::iterator i = s.begin();
	if (i == s.end())
		return "[]";
	string r = "[\n" + i->first + ":" + ToString(i->second) + "\n";
	++i;
	while (i != s.end())
	{
		r += i->first + ":" + ToString(i->second) + "\n";
		++i;
	}
	return r + "]";
}
