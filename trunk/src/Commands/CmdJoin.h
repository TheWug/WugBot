#ifndef _CMDJOIN_H_
#define _CMDJOIN_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdJoin : public cmd
{
	public:
	CmdJoin();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	void ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif