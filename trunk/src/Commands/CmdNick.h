#ifndef _CMDNICK_H_
#define _CMDNICK_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdNick : public cmd
{
	public:
	CmdNick();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
