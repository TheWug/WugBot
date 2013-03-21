#ifndef _CMDDNS_H_
#define _CMDDNS_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdDNS : public cmd
{
	public:
	CmdDNS();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();
};

#endif
