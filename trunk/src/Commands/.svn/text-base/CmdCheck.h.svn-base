#ifndef _CMDCHECK_H_
#define _CMDCHECK_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class CmdCheck : public cmd
{
	public:
	CmdCheck();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	vector<string> CommandStrings();

	void Dispatch(IRCBot& bot, Hostname speaker, string target, string server);
};

#endif
