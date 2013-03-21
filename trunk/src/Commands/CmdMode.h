#ifndef _CMDMODE_H_
#define _CMDMODE_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdMode : public cmd
{
	public:
	string modes;
	CmdMode();

	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	void ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);
	void PostInstall(IRCBot& bot);

	private:
	vector<string> CommandStrings();
};

#endif
