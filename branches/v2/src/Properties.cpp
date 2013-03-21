#include <fstream>
#include <unistd.h>

#include "Properties.h"

#include "BotLogger.h"
#include "BotCore.h"
#include "StringUtil.h"
#include "VectorUtil.h"

Properties::Properties(string filename, BotCore& b) : bot(b)
{
	myfile = filename;
	bot.BotLog().GetLog(BotLogger::SYS).Put(INFO, "Properties: instantiated, bound to " + myfile);
}

bool Properties::Read()
{
	ifstream props;
	props.open(myfile.c_str(), ios_base::in);
	string line;

	string section;
	string name;
	string value;

	if (props.fail())
	{
		props.close();
		bot.BotLog().GetLog(BotLogger::SYS).Put(CRITICAL, "Properties: UNABLE TO READ TO PROPERTIES FILE: " + myfile);
		return true;
	}

	properties.clear();

	while (!props.eof())
	{
		getline(props, line);
		bot.BotLog().GetLog(BotLogger::DBG).Put(INFO, "Properties: read line: " + line);
		if (line.find("[") == 0 && line.find("]") == line.length() - 1)
		{
			section = StringUtil::ToUpper(line.substr(1, line.length() - 2));
			bot.BotLog().GetLog(BotLogger::DBG).Put(INFO, "Properties: read section: " + section);
		}
		else if (line.find("=") != string::npos)
		{
			name = StringUtil::StripSymbols(StringUtil::ToLower(line.substr(0, line.find("="))));
			value = line.substr(line.find("=") + 1, line.length() - name.length() - 1);
			properties[section][name] = value;
			bot.BotLog().GetLog(BotLogger::DBG).Put(INFO, "Properties: read value: " + section + "/" + name + "=" + value);
		}
	}
	props.close();

	bot.BotLog().GetLog(BotLogger::SYS).Put(INFO, "Properties: read properties from " + myfile);
	return false;
}

bool Properties::Write()
{
	ofstream newprops;
	string tempname = (myfile == "temp.conf") ? "tempx.conf" : "temp.conf";

	newprops.open(tempname.c_str(), ios_base::trunc | ios_base::out);
	if (newprops.fail())
	{
		newprops.close();
		bot.BotLog().GetLog(BotLogger::SYS).Put(CRITICAL, "Properties: UNABLE TO WRITE TO PROPERTIES FILE: " + myfile);
		return true;
	}

	for (map<string, map<string, string> >::iterator sec = properties.begin(); sec != properties.end(); ++sec)
	{
		newprops << "[" << StringUtil::ToUpper(sec->first) << "]" << endl;
		for (map<string, string>::iterator nam = sec->second.begin(); nam != sec->second.end(); ++nam)
		{
			newprops << StringUtil::StripSymbols(StringUtil::ToLower(nam->first)) << "=" << StringUtil::StripNewlines(nam->second) << endl;
		}
	}
	newprops.close();

	unlink(myfile.c_str());
	rename(tempname.c_str(), myfile.c_str());
	bot.BotLog().GetLog(BotLogger::SYS).Put(INFO, "Properties: wrote properties to " + myfile);
	return false;
}

string Properties::GetProperty(string name, string section, string def)
{
	if (properties.find(section) == properties.end()) return def;
	if (properties[section].find(name) == properties[section].end()) return def;
	return properties[section][name];
}

vector<string> Properties::GetProperty(string name, string section, vector<string> def)
{
	if (properties.find(section) == properties.end()) return def;
	if (properties[section].find(name) == properties[section].end()) return def;
	return StringUtil::Vectorize(properties[section][name]);
}

int Properties::GetProperty(string name, string section, int def)
{
	if (properties.find(section) == properties.end()) return def;
	if (properties[section].find(name) == properties[section].end()) return def;
	return StringUtil::ToInt32(properties[section][name]);
}

vector<int> Properties::GetProperty(string name, string section, vector<int> def)
{
	if (properties.find(section) == properties.end()) return def;
	if (properties[section].find(name) == properties[section].end()) return def;
	return VectorUtil::ToInt32(StringUtil::Vectorize(properties[section][name]));
}

double Properties::GetProperty(string name, string section, double def)
{
	if (properties.find(section) == properties.end()) return def;
	if (properties[section].find(name) == properties[section].end()) return def;
	return StringUtil::ToInt32(properties[section][name]);
}

vector<double> Properties::GetProperty(string name, string section, vector<double> def)
{
	if (properties.find(section) == properties.end()) return def;
	if (properties[section].find(name) == properties[section].end()) return def;
	return VectorUtil::ToDouble(StringUtil::Vectorize(properties[section][name]));
}

void Properties::SetProperty(string name, string section, string value)
{
}

void Properties::SetProperty(string name, string section, vector<string> value)
{
}

void Properties::SetProperty(string name, string section, int value)
{
}

void Properties::SetProperty(string name, string section, vector<int> value)
{
}

void Properties::SetProperty(string name, string section, double value)
{
}

void Properties::SetProperty(string name, string section, vector<double> value)
{
}
