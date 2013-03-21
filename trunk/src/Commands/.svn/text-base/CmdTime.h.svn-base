#ifndef _CMDTIME_H_
#define _CMDTIME_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdTime : public cmd
{
	public:
	CmdTime();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();

	string GetUptime();
	string GetSystemTime();
};

#endif
