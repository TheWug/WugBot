#include "StringUtil.h"

string StringUtil::StripWS(string x)
{
	return x;
}

string StringUtil::ToUpper(string x)
{
	return x;
}

string StringUtil::ToLower(string x)
{
	return x;
}

string StringUtil::StripSymbols(string x)
{
	return x;
}

string StringUtil::StripNewlines(string x)
{
	return x;
}

vector<string> StringUtil::Vectorize(string x)
{
	return vector<string>();
}

int StringUtil::ToInt32(string x)
{
	return 0;
}

string StringUtil::FromDouble(double x)
{
	return "FIXME";
}

string StringUtil::FromInt32(int x)
{
	return "FIXME";
}

string StringUtil::FromIPAddr(int x)
{
	return "F.I.X.ME";
}

bool StringUtil::CaseInsEquals(string a, string b)
{
	return ToLower(a) == ToLower(b);
}

bool StringUtil::IRCEquals(string a, string b)
{
	return false;
}

string StringUtil::FormatTime(string format, unsigned long long time)
{
	return format;
}

string StringUtil::FormatTime(string format, timespec time)
{
	return format;
}
