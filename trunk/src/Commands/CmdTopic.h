#ifndef _CMDTOPIC_H_
#define _CMDTOPIC_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdTopic : public cmd
{
	public:
	CmdTopic();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	void ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
