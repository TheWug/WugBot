#ifndef _CMDFACTOID_H_
#define _CMDFACTOID_H_

#include <string>
#include <vector>

#include "cmd.h"

class FactoidProcessor;

using namespace std;

class CmdFactoid : public cmd // all things factoid
{
	public:
	IRCBot& bot;
	FactoidProcessor * factoidprocessor;

	CmdFactoid(IRCBot& b);
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);
	void PostInstall(IRCBot& bot);
	void SaveState();

	private:
	vector<string> CommandStrings();
};

#endif
