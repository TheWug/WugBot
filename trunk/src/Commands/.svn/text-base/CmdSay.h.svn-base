#ifndef _CMDSAY_H_
#define _CMDSAY_H_

#include <string>
#include <vector>
#include <map>

#include "cmd.h"

using namespace std;

class CmdSay : public cmd
{
	public:
	CmdSay();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);
	void Say(IRCBot& bot, string command, string target, string message);

	private:
	vector<string> CommandStrings();

	map<string, string> watchedchannels;
};

#endif
