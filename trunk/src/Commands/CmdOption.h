#ifndef _CMDOPTION_H_
#define _CMDOPTION_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdOption : public cmd
{
	public:
	CmdOption();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
