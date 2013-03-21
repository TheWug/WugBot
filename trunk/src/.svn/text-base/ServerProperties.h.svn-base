#ifndef _SERVERCAPS_H_
#define _SERVERCAPS_H_

#include <string>
#include <map>

using namespace std;

class ServerProperties
{
	public:
	ServerProperties();

	void UnsetProperty(string name);
	void SetProperty(string name, string property);
	string GetProperty(string name);
	int GetPropertyInt(string name);
	bool IsPropertySet(string name);

	private:
	map<string, string> properties;
};

#endif
