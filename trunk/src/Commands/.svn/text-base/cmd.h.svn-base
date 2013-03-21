#ifndef _CMD_H_
#define _CMD_H_

#include <string>
#include <vector>

#include "Hostname.h"

using namespace std;

class IRCBot;

class cmd
{
	public:
	int defaultuserlevel;
	string defaultuserflags;
	bool acceptsflags;
	cmd();
	virtual ~cmd() {}

	void Register(IRCBot& bot);

	virtual vector<string> CommandStrings() = 0;
	virtual void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args) = 0;
	virtual void ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	virtual string HelpMsg(string command);
	virtual void PostInstall(IRCBot& bot);
	virtual void SaveState();
	virtual bool IsAlias();
	bool AcceptsFlags();
	void FlagTestAndGo(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args, string channel);
};

#endif
