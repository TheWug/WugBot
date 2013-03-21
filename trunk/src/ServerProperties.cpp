#include "ServerProperties.h"
#include "misc.h"

ServerProperties::ServerProperties()
{
	properties["nicknamechars"] = "abcdefghijklmnopqrstuvwxyz{}|~ABCDEFGHIJKLMNOPQRSTUVWXYZ[]\\^";
	properties["channelnamechars"] = "abcdefghijklmnopqrstuvwxyz{}|~ABCDEFGHIJKLMNOPQRSTUVWXYZ[]\\^";
	properties["chantypes"] = "#";
	properties["monitor"] = "0";
}

void ServerProperties::UnsetProperty(string name)
{
	properties.erase(tolower(name));
}

void ServerProperties::SetProperty(string name, string property)
{
	properties[tolower(name)] = property;
}

string ServerProperties::GetProperty(string name)
{
	return properties[tolower(name)];
}

int ServerProperties::GetPropertyInt(string name)
{
	istringstream s(properties[tolower(name)]);
	int i = 0xFFFFFFFF;
	s >> i;
	return i;
}

bool ServerProperties::IsPropertySet(string name)
{
	return (properties.find(tolower(name)) != properties.end());
}


