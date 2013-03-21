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
	static string StripWS(string x);
	static string ToUpper(string x);
	static string ToLower(string x);
	static string StripSymbols(string x);
	static string StripNewlines(string x);

	// seperation
	static vector<string> Vectorize(string x);

	// casts from string
	static double ToDouble(string x);
	static int ToInt32(string x);
	static int ToIPAddr(string x);

	// casts to string
	static string FromDouble(double x);
	static string FromInt32(int x);
	static string FromIPAddr(int x);

	// equality
	static bool CaseInsEquals(string a, string b);
	static bool IRCEquals(string a, string b);

	// time
	static string FormatTime(string format, unsigned long long time);
	static string FormatTime(string format, timespec time);
};

#endif
