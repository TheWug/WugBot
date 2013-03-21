#ifndef _CMDCTCPRESP_H_
#define _CMDCTCPRESP_H_

#include <vector>
#include <string>

#include "cmd.h"

using namespace std;

class IRCBot;

class CmdCtcpResp : public cmd
{
	public:
	IRCBot& mybot;

	CmdCtcpResp(IRCBot& b);
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	vector<string> CommandStrings();
	void PostInstall(IRCBot& bot);
	void SaveState();
};

#endif
