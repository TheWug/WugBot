#ifndef _VECTOR_UTILS_H_
#define _VECTOR_UTILS_H_

#include <string>
#include <vector>

using namespace std;

class VectorUtil
{
public:
	static vector<int> ToInt32(vector<string> x);
	static vector<double> ToDouble(vector<string> x);

	static vector<string> FromInt32(vector<int> x);
	static vector<string> FromDouble(vector<double> x);

	static string Join(vector<string> x, string delim);
};

#endif
