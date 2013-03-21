#ifndef _CMDON_H_
#define _CMDON_H_

#include <string>
#include <vector>

#include "cmd.h"

using namespace std;

class MessageHandler;

class CmdOn : public cmd
{
	public:
	CmdOn(IRCBot& bot);
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);
	
	private:
	MessageHandler * onhandler;
	vector<string> CommandStrings();
};

#endif
