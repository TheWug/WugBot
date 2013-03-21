#ifndef _CMDHELP_H_
#define _CMDHELP_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdHelp : public cmd
{
	public:
	CmdHelp();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string GeneralHelpMessage(IRCBot& bot);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
