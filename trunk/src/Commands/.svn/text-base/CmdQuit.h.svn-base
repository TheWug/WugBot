#ifndef _CMDQUIT_H_
#define _CMDQUIT_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdQuit : public cmd
{
	public:
	CmdQuit();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
