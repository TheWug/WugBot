#ifndef _CMDFLAGS_H_
#define _CMDFLAGS_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdFlags : public cmd
{
	public:
	CmdFlags();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	void ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	string availableflags;
	vector<string> CommandStrings();
};

#endif
