#include "VectorUtil.h"

#include <limits>

#include "StringUtil.h"

vector<int> VectorUtil::ToInt32(vector<string> x)
{
	vector<int> vi;
	for (vector<string>::iterator i = x.begin(); i != x.end(); ++i)
	{
		try
		{
			vi.push_back(StringUtil::ToInt32(*i));
		}
		catch (exception& e)
		{
			vi.push_back(-1);
		}
	}
	return vi;
}

vector<double> VectorUtil::ToDouble(vector<string> x)
{
	vector<double> vd;
	for (vector<string>::iterator i = x.begin(); i != x.end(); ++i)
	{
		try
		{
			vd.push_back(StringUtil::ToDouble(*i));
		}
		catch (exception& e)
		{
			vd.push_back(numeric_limits<double>::quiet_NaN()); 
		}
	}
	return vd;
}

vector<string> VectorUtil::FromInt32(vector<int> x)
{
	vector<string> v;
	for (vector<int>::iterator i = x.begin(); i != x.end(); ++i) v.push_back(StringUtil::FromInt32(*i));
	return v;
}

vector<string> VectorUtil::FromDouble(vector<double> x)
{
	vector<string> v;
	for (vector<double>::iterator i = x.begin(); i != x.end(); ++i) v.push_back(StringUtil::FromDouble(*i));
	return v;
}

string VectorUtil::Join(vector<string> x, string delim)
{
	string s;
	if (x.size() > 0) s += x[0];
	for (vector<string>::iterator i = ++x.begin(); i != x.end(); ++i) (s += delim) += *i;
	return s;
}
