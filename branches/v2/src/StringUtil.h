#ifndef _STRING_UTIL_H_
#define _STRING_UTIL_H_

#include <string>
#include <vector>
#include <ctime>

using namespace std;

class StringUtil
{
public:
	// formatting stuff
	static string GenericStrip(string x, string remove);
	static string StripWS(string x);
	static string ToUpper(string x);
	static string ToLower(string x);
	static string StripSymbols(string x);
	static string StripNewlines(string x);
	static string Replace(string original, vector<string> finds, vector<string> replacements);
	static string Replace(string original, string find, string replace);

	// seperation
	static vector<string>& Vectorize(string s, char delim, vector<string>& elems);
	static vector<string> Vectorize(string s, char delim);
	static vector<string> Vectorize(string x);

	// casts from string
	static double ToDouble(string x);
	static int ToInt32(string x);
	static unsigned int ToUInt32(string x);
	static unsigned int ToIPAddr(string x);

	// casts to string
	static string FromDouble(double x);
	static string FromInt32(int x);
	static string FromUInt32(unsigned int x);
	static string FromIPAddr(unsigned int x);

	// equality
	static bool CaseInsEquals(string a, string b);
	static bool IRCEquals(string a, string b);

	// time (note that the format symbols for time and interval formatting functions are different.)
	// FormatTime uses strftime and thus uses the exact same format sequence set (and %nnn for 3 digit milliseconds, which isn't provided by default)
	// FormatTimeInterval uses nnn, NNN, ss, SS, mm, MM, hh, HH, dd, DD, yy, YY (lower case is mod, upper case is total) to represent msec, sec, min, hr, day, year
	static string FormatTime(string format, unsigned long long time);
	static string FormatTime(string format, timespec time);
	static string FormatTimeInterval(string format, unsigned long long time);
	static string FormatTimeInterval(string format, timespec time);
};

#endif
