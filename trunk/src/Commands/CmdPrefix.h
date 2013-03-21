#ifndef _CMDPREFIX_H_
#define _CMDPREFIX_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdPrefix : public cmd
{
	public:
	CmdPrefix();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
