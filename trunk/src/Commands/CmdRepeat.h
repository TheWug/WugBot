#ifndef _CMDREPEAT_H_
#define _CMDREPEAT_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdRepeat : public cmd
{
	public:
	CmdRepeat();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
