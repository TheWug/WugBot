#ifndef _CMDRAW_H_
#define _CMDRAW_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdRaw : public cmd
{
	public:
	CmdRaw();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
