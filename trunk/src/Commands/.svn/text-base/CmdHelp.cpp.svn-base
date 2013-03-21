#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "CmdHelp.h"
#include "IRCBot.h"

CmdHelp::CmdHelp()
{
	defaultuserlevel = 0;
}

vector<string> CmdHelp::CommandStrings()
{
	vector<string> types;
	types.push_back("help");
	return types;
}

void CmdHelp::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream co(args);
	co >> args;
	map<string, cmd *>::iterator c = bot.commands.find(args);
	if (args == "")
	{
		bot.Say(speaker.GetNick(), GeneralHelpMessage(bot));
		return;
	}
	if (c == bot.commands.end())
	{
		bot.Say(speaker.GetNick(), "Command " + args + " does not exist.  Try ;help for a list of commands.");
		return;
	}
	else
	{
		bot.Say(speaker.GetNick(), c->second->HelpMsg(args));
		return;
	}
}

string CmdHelp::HelpMsg(string command)
{
	return "Usage: help [command] -- displays help about the specified command.  If no command is specified, general help is printed and all commands are listed.";
}

string CmdHelp::GeneralHelpMessage(IRCBot& bot)
{
	string message = "Hello! I'm a bot.  My command prefix is ';' or '[MYNAME], '.  A full list of commands is below.  To learn about one, use ';help <command>'.\r\n";
	for (map<string, cmd *>::iterator i = bot.commands.begin(); i != bot.commands.end(); i++, message.append(" "))
		message.append(i->first);
	return message;
}
