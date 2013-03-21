#ifndef _CMDTELL_H_
#define _CMDTELL_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdTell : public cmd
{
	public:
	CmdTell();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	void Say(IRCBot& bot, string command, string target, string message);
	vector<string> CommandStrings();
};

#endif
