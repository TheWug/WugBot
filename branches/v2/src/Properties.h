#ifndef _PROPERTIES_H_
#define _PROPERTIES_H_

#include <vector>
#include <string>
#include <map>

#include "EventHandler.h"

using namespace std;

class BotCore;

class Properties
{
private:
	BotCore& bot;
	string myfile;
	map<string, map<string, string> > properties;

	static void PropertyHandler(void * s, EventHandler::Event e);
public:
	Properties(string filename, BotCore& b);

	bool Read();
	bool Write();

	string GetProperty(string name, string section, string def);
	vector<string> GetProperty(string name, string section, vector<string> def);
	int GetProperty(string name, string section, int def);
	vector<int> GetProperty(string name, string section, vector<int> def);
	double GetProperty(string name, string section, double def);
	vector<double> GetProperty(string name, string section, vector<double> def);

	void SetProperty(string name, string section, string value);
	void SetProperty(string name, string section, vector<string> value);
	void SetProperty(string name, string section, int value);
	void SetProperty(string name, string section, vector<int> value);
	void SetProperty(string name, string section, double value);
	void SetProperty(string name, string section, vector<double> value);

	void SubscribeToEvents();
};

#endif
