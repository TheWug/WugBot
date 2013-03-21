#ifndef _CMDACCESS_H_
#define _CMDACCESS_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdAccess : public cmd
{
	public:
	CmdAccess();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();

	void Mute(IRCBot& bot, Hostname& speaker, string respondto, string subcommand, string args);
	void Ban(IRCBot& bot, Hostname& speaker, string respondto, string subcommand, string args);
	void Ignore(IRCBot& bot, Hostname& speaker, string target, string respondto, string subcommand, string args);
	void Chat(IRCBot& bot, Hostname& speaker, string respondto, string subcommand, string args);
};

#endif
