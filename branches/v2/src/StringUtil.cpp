#include "StringUtil.h"

#include <sstream>
#include <algorithm>
#include <cctype>
#include <exception>
#include <stdexcept>
#include <ctime>

#include "BotTime.h"

string StringUtil::GenericStrip(string x, string remove)
{
	int start = x.find_first_not_of(remove);
	if (start == -1) return "";
	int end = x.find_last_not_of(remove);
	return x.substr(start, end - start + 1);
}

string StringUtil::StripWS(string x)
{
	return GenericStrip(x, " \t\r\n\v\f");
}

string StringUtil::ToUpper(string x)
{
	transform(x.begin(), x.end(), x.begin(), (int (*)(int)) toupper);
	return x;
}

string StringUtil::ToLower(string x)
{
	transform(x.begin(), x.end(), x.begin(), (int (*)(int)) tolower);
	return x;
}

string StringUtil::StripSymbols(string x)
{
	return GenericStrip(x, "`~!@#$%^&*()-=_+[]{},./<>?;':\"|\\");
}

string StringUtil::StripNewlines(string x)
{
	return GenericStrip(x, "\r\n");;
}

string StringUtil::Replace(string original, vector<string> finds, vector<string> replacements)
{
	int maximum = min(finds.size(), replacements.size());
	for (int i = 0; i < maximum; ++i) original = Replace(original, finds[i], replacements[i]);
	return original;
}

string StringUtil::Replace(string original, string find, string replace)
{
	int pos;
	while ((pos = original.find(find)) != string::npos) original.replace(pos, find.length(), replace);
	return original;
}

vector<string>& StringUtil::Vectorize(string x, char delim, vector<string>& elems)
{
	stringstream ss(x);
	string item;
	while(std::getline(ss, item, delim)) elems.push_back(item);
	return elems;
}


vector<string> StringUtil::Vectorize(string x, char delim)
{
	vector<std::string> elems;
	return Vectorize(x, delim, elems);
}

vector<string> StringUtil::Vectorize(string x)
{
	vector<std::string> elems;
	return Vectorize(x, ',', elems);
}

double StringUtil::ToDouble(string x)
{
	istringstream ss(x);
	double d;
	ss >> d;
	if (!ss) throw domain_error("StringUtil::ToDouble: Could not cast specified string to double");
	return d;
}

int StringUtil::ToInt32(string x)
{
	istringstream ss(x);
	int i;
	ss >> i;
	if (!ss) throw domain_error("StringUtil::ToInt32: Could not cast specified string to integer");
	return i;
}

unsigned int StringUtil::ToUInt32(string x)
{
	istringstream ss(x);
	unsigned int i;
	ss >> i;
	if (!ss) throw domain_error("StringUtil::ToUInt32: Could not cast specified string to integer");
	return i;
}

unsigned int StringUtil::ToIPAddr(string x)
{
	static domain_error d("StringUtil::ToIPAddr: Could not cast specified string to IP address");
	istringstream ss(x);
	char c;
	int i1, i2, i3, i4;
	ss >> i1; if (!ss) throw d;
	ss >> c; if (!ss || c != '.') throw d;
	ss >> i2; if (!ss) throw d;
	ss >> c; if (!ss || c != '.') throw d;
	ss >> i3; if (!ss) throw d;
	ss >> c; if (!ss || c != '.') throw d;
	ss >> i4; if (!ss) throw d;
	if (i1 < 0 || i1 > 255 || i2 < 0 || i2 > 255 || i3 < 0 || i3 > 255 || i4 < 0 || i4 > 255) throw d;
	return (i1 << 24) | (i2 << 16) | (i3 << 8) | (i4);
}

string StringUtil::FromDouble(double x)
{
	ostringstream ss;
	ss << x;
	return ss.str();
}

string StringUtil::FromInt32(int x)
{
	ostringstream ss;
	ss << x;
	return ss.str();
}

string StringUtil::FromUInt32(unsigned int x)
{
	ostringstream ss;
	ss << x;
	return ss.str();
}

string StringUtil::FromIPAddr(unsigned int x)
{
	int i1 = (x >> 24) & 0xFF, i2 = (x >> 16) & 0xFF, i3 = (x >> 8) & 0xFF, i4 = (x) & 0xFF;
	return FromInt32(i1) + "." + FromInt32(i2) + "." + FromInt32(i3) + "." + FromInt32(i4);
}

bool StringUtil::CaseInsEquals(string a, string b)
{
	return ToLower(a) == ToLower(b);
}

bool StringUtil::IRCEquals(string a, string b) // RFC1459, plz gfys
{
	return false;
}

string StringUtil::FormatTime(string format, unsigned long long time)
{
	return FormatTime(format, BotTime::GetMillisAsTimespec(time));
}

string StringUtil::FormatTime(string format, timespec time)
{
	tm represented;
	char buffer[64];

	localtime_r(&time.tv_sec, &represented);

	format = Replace(format, "%nnn", FromInt32(time.tv_nsec / 1000000));
	strftime(buffer, sizeof(buffer), format.c_str(), &represented);
	return string(buffer);
}

string StringUtil::FormatTimeInterval(string format, unsigned long long time)
{
	unsigned int tmillis, tseconds, tminutes, thours, tdays, tyears;
	unsigned int millis, seconds, minutes, hours, days, years;
	tmillis = time;
	millis = tmillis % 1000;
	time /= 1000;
	tseconds = time;
	seconds = tseconds % 60;
	time /= 60;
	tminutes = time;
	minutes = tminutes % 60;
	time /= 60;
	thours = time;
	hours = thours % 24;
	time /= 24;
	tdays = time;
	days = tdays % 365;
	time /= 365;
	tyears = time;
	years = tyears;

	vector<string> finds, replaces;
	finds.push_back("nnn"); replaces.push_back((millis < 100 ? (millis < 10 ? "00" : "0") : "") + FromInt32(millis));
	finds.push_back("NNN"); replaces.push_back(FromInt32(tmillis));
	finds.push_back("sss"); replaces.push_back((seconds < 10 ? "0" : "") + FromInt32(seconds));
	finds.push_back("SSS"); replaces.push_back(FromInt32(tseconds));
	finds.push_back("mmm"); replaces.push_back((minutes < 10 ? "0" : "") + FromInt32(minutes));
	finds.push_back("MMM"); replaces.push_back(FromInt32(tminutes));
	finds.push_back("hhh"); replaces.push_back((hours < 10 ? "0" : "") + FromInt32(hours));
	finds.push_back("HHH"); replaces.push_back(FromInt32(thours));
	finds.push_back("ddd"); replaces.push_back(FromInt32(days));
	finds.push_back("DDD"); replaces.push_back(FromInt32(tdays));
	finds.push_back("yyy"); replaces.push_back(FromInt32(years));
	finds.push_back("YYY"); replaces.push_back(FromInt32(tyears));
	format = Replace(format, finds, replaces);

	return format;
}

string StringUtil::FormatTimeInterval(string format, timespec time)
{
	return FormatTimeInterval(format, BotTime::GetTimespecAsMillis(time));
}
