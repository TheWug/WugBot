#ifndef _CMDSAVE_H_
#define _CMDSAVE_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdSave : public cmd
{
	public:
	CmdSave();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
