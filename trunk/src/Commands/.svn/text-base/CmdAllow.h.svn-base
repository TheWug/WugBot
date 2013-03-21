#ifndef _CMDALLOW_H_
#define _CMDALLOW_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdAllow : public cmd
{
	public:
	CmdAllow();
	void PostInstall(IRCBot& bot);
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
