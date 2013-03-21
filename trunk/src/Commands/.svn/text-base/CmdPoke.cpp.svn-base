#include <iostream>
#include <stdlib.h>

#include "CmdPoke.h"
#include "IRCBot.h"

CmdPoke::CmdPoke()
{
	defaultuserlevel = 0;
}

vector<string> CmdPoke::CommandStrings()
{
	vector<string> types;
	types.push_back("poke");
	types.push_back("ping");
	types.push_back("ding");
	return types;
}

void CmdPoke::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	bot.Say(respondto, speaker.GetNick() + ": " + command);
}

string CmdPoke::HelpMsg(string command)
{
	return "Usage: " + command + " -- Checks the responsiveness of the bot.";
}
