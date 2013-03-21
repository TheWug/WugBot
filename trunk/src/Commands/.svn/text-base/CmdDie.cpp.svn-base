#include "CmdDie.h"

CmdDie::CmdDie()
{
	defaultuserlevel = 5;
}

void CmdDie::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	*((int *) 0) = 0;
}

string CmdDie::HelpMsg(string command)
{
	return "Usage: die-a-horrible-death -- causes the bot to encounter a segmentation fault and close.";
}

vector<string> CmdDie::CommandStrings()
{
	vector<string> types;
	types.push_back("die-a-horrible-death");
	return types;
}
