#ifndef _ALIAS_H_
#define _ALIAS_H_

#include <string>
#include <vector>

#include "cmd.h"

class IRCBot;

using namespace std;

class alias : public cmd
{
	public:

	vector<string> commands;
	string helpmsg;
	string command;
	int channelarg;

	public:
	alias(string c);
	bool IsAlias();
	void SetHelpMsg(string message);
	void AddCommand(string command);
	void SetChannelArgument(int i);
	void Clear();

	vector<string> CommandStrings();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	void ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);
	void Unregister(IRCBot& bot);
	void ReplaceTokensWithArgs(string& haystack, string argstring);
};

#endif
