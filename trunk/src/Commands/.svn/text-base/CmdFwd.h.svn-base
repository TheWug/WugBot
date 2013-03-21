#ifndef _CMDFWD_H_
#define _CMDFWD_H_

#include <string>
#include <vector>

#include "cmd.h"

class dcmd;

using namespace std;

class CmdFwd : public cmd
{
	public:
	CmdFwd();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:

	void Add(IRCBot& bot, Hostname speaker, string target, string respondto, string args);
	void Remove(IRCBot& bot, Hostname speaker, string target, string respondto, string args);
	void RemoveAll(IRCBot& bot, Hostname speaker, string target, string respondto, string args);

	vector<string> CommandStrings();

	vector<dcmd *> forwardlist;
};

#endif
