#ifndef _CMDHASPAID_H_
#define _CMDHASPAID_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdHaspaid : public cmd
{
	public:
	CmdHaspaid();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);
	void PostInstall(IRCBot& bot);
	void SaveState();

	string GetURL(string user);

	bool HasPaid(string user) throw (string);

	private:
	vector<string> CommandStrings();
	string haspaidURL;
};

#endif
