#ifndef _CMDALIAS_H_
#define _CMDALIAS_H_

#include <string>
#include <vector>
#include <set>

#include "cmd.h"

using namespace std;

class alias;

class CmdAlias : public cmd
{
	public:
	CmdAlias();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	void PostInstall(IRCBot& bot);
	void SaveState();

	private:
	void New(IRCBot& bot, string name, string args, string respondto);
	void Delete(IRCBot& bot, string name, string args, string respondto);
	void Add(IRCBot& bot, string name, string args, string respondto);
	void Clear(IRCBot& bot, string name, string args, string respondto);
	void SetHelp(IRCBot& bot, string name, string args, string respondto);
	void Check(IRCBot& bot, string name, string args, string respondto);
	void SetChannelArgument(IRCBot& bot, string name, string args, string respondto);
	vector<string> CommandStrings();

	set<alias *> aliases;
};

#endif
