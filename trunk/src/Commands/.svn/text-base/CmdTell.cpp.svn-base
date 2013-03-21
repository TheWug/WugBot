#include <iostream>
#include <stdlib.h>

#include "CmdTell.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"

CmdTell::CmdTell()
{
	defaultuserlevel = 1;
}

void CmdTell::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream resp(args);
	string telltarget;
	resp >> telltarget;
	args = getline(resp);
	if (telltarget == "")
		bot.Say(respondto, "Tell who what?");
	else if (!bot.permission.CanSendTo(telltarget))
		bot.Say(respondto, "I can't speak to them.");
	else
		Say(bot, command, telltarget, args);
	return;
}

vector<string> CmdTell::CommandStrings()
{
	vector<string> types;
	types.push_back("tell");
	types.push_back("tell-notice");
	types.push_back("tell-msg");
	types.push_back("tell-act");
	return types;
}

string CmdTell::HelpMsg(string command)
{
	return "Usage: tell <target> <message> -- Makes the bot send the provided message to the provided target, which can be a user or a channel.";
}

void CmdTell::Say(IRCBot& bot, string command, string target, string message)
{
	if (command == "tell")
		bot.Say(target, message);
	else if (command == "tell-msg")
		bot.Message(target, message);
	else if (command == "tell-notice")
		bot.Notice(target, message);
	else if (command == "tell-act")
		bot.Action(target, message);
}
