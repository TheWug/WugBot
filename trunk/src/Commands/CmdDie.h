#ifndef _CMDDIE_H_
#define _CMDDIE_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdDie : public cmd
{
	public:
	CmdDie();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
