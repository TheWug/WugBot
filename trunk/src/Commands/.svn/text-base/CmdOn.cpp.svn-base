#include <iostream>
#include <stdlib.h>

#include "CmdOn.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"
#include "DCmd.h"

CmdOn::CmdOn(IRCBot& bot)
{
	onhandler = new MessageHandler(bot);
	bot.AddDeferredCommand(onhandler);
	defaultuserlevel = 3;
}

vector<string> CmdOn::CommandStrings()
{
	vector<string> types;
	types.push_back("on-add");
	types.push_back("on-remove");
	return types;
}

void CmdOn::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream str(args);
	string servermessage, servertarget, serverorigin, commandtocall, commandstring;
	str >> servermessage;
	str >> servertarget;
	str >> serverorigin;
	servermessage = tolower(servermessage);
	servertarget = tolower(servertarget);
	serverorigin = tolower(serverorigin);

	if (command == "on-add")
	{
		str >> commandtocall;
		commandtocall = tolower(commandtocall);
		commandstring = getline(str);
		if (bot.commands.find(commandtocall) == bot.commands.end())
		{
			bot.Say(respondto, "Invalid command: " + commandtocall);
			return;
		}
		if (!bot.permission.CanUseCommand(speaker.GetAccountL(), command))
		{
			bot.Say(respondto, "You do not have permission to use that command.");
			return;
		}
		onhandler->SetCommand(servermessage, servertarget, serverorigin, commandtocall + " " + commandstring);
		return;
	}
	else if (command == "on-remove")
	{
		onhandler->ClearCommand(servermessage, servertarget, serverorigin);
		return;
	}
	return;
}

string CmdOn::HelpMsg(string command)
{
	return "Usage: " + command + " <message> <target> <speaker> <command to call> -- calls the specified command when the bot recieves the specified message from the server (directed at the specified target, from the specified speaker).\
		Message should be a string sent by the server (PRIVMSG, JOIN, etc). To specify any target or speaker, specify a *.  You must have permission to execute the command you specify.";
}
