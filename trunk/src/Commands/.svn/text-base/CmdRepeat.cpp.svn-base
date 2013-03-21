#include <iostream>
#include <stdlib.h>

#include "CmdRepeat.h"
#include "IRCBot.h"
#include "misc.h"

CmdRepeat::CmdRepeat()
{
	defaultuserlevel = -1;
}

vector<string> CmdRepeat::CommandStrings()
{
	vector<string> types;
	types.push_back("repeat");
	return types;
}

void CmdRepeat::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream t(args);
	int quantity = -1;
	t >> quantity;

	if (quantity < 0 || quantity > 10)
	{
		bot.Say(respondto, "You can't do that.  You can only repeat something between 0 and 10 times.");
		return;
	}

	args = getline(t);

	for (int i = 0; i < quantity; ++i)
		bot.PerformCommand(speaker, target, respondto, args);
}

string CmdRepeat::HelpMsg(string command)
{
	return "Usage: repeat <num> <command ... > -- execute a command multiple times.  Only really useful for spamming.  num must be between 0 and 10, inclusive.";
}
