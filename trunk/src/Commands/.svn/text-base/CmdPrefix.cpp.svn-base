#include <iostream>
#include <sstream>

#include "CmdPrefix.h"
#include "IRCBot.h"

CmdPrefix::CmdPrefix()
{
	defaultuserlevel = 5;
}

void CmdPrefix::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream temp(args);
	string dummy;
	temp >> dummy;
	if (dummy.length() != 0)
	{
		bot.Say(respondto, string("Bot command prefix changed to '") + dummy[0] + "' (was '" + bot.commandprefix + "')");
		bot.commandprefix = dummy[0];
	}
	else
		bot.Say(respondto, string("Bot's command prefix is currently '") + bot.commandprefix + "'");
}

string CmdPrefix::HelpMsg(string command)
{
	return "Usage: prefix [new prefix character] -- displays or sets the bot's command prefix character.";
}

vector<string> CmdPrefix::CommandStrings()
{
	vector<string> types;
	types.push_back("prefix");
	return types;
}
