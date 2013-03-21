#ifndef _CMDWHERE_H_
#define _CMDWHERE_H_

#include <vector>
#include <string>

#include "cmd.h"

using namespace std;

class CmdWhere : public cmd
{
	public:
	CmdWhere();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
