#ifndef _CMDMEMO_H_
#define _CMDMEMO_H_

#include <string>
#include <vector>

#include "cmd.h"

class IRCBot;
class MemoSender;

class CmdMemo : public cmd
{
	public:
	CmdMemo(IRCBot& bot);
	~CmdMemo();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	private:
	MemoSender& memosender;
	vector<string> CommandStrings();
};

#endif
