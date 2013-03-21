#include <iostream>
#include <stdlib.h>

#include "CmdAllow.h"
#include "DeferredAllow.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "UserManager.h"
#include "misc.h"

CmdAllow::CmdAllow()
{
	defaultuserlevel = 999;
}

void CmdAllow::PostInstall(IRCBot& bot)
{
	string s = "ul";
	bot.permission.SetDefaultCommandLevel(s, 0);
	s = "cl";
	bot.permission.SetDefaultCommandLevel(s, 0);
}

void CmdAllow::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> data = splitcommands(args, " ", false);
	if (data.size() == 0)
	{
		bot.Say(respondto, "Not enough parameters, specify both a subject and optionally a number.");
		return;
	}
	if (command == "allow" || command == "ul")
	{
		int newlevel = -1;
		newlevel = atoi(data[1].c_str());
		if (newlevel < 0)
		{
			bot.Say(respondto, "Invalid level specified.");
			return;
		}
		string h;
		if (data[0].at(0) == '~')
			h = tolower(data[0].c_str() + 1);
		else if (data[0].at(0) == '*')
			h = "*";
		else
		{
			h = bot.usermgr.GetUsernameFromNick(data[0]);
			if (h == "")
			{
				bot.AddDeferredCommand(new DeferredAllow(speaker, target, bot, args, command));
				return;
			}
			if (h == "*" && command == "allow")
			{
				bot.Say(respondto, "User is not logged in.  To set permissions for unauthenticated users, use '" + command + " *'.");
				return;
			}
		}
		stringstream out;
		if (data.size() == 1 || command == "ul")
			out << "User " << h << " has userlevel " << bot.permission.GetUserLevel(h) << ".";
		else
		{
			out << "User " << h << " has been given userlevel " << newlevel << ".";
			bot.permission.SetUserLevel(tolower(h), newlevel);
		}
		bot.Say(respondto, out.str());
	}
	else if (command == "command" || command == "cl")
	{
		data[0] = tolower(data[0]);
		if (bot.commands.find(data[0]) == bot.commands.end())
		{
			bot.Say(respondto, "Nonexistant Command.");
			return;
		}
		stringstream out;
		if (data.size() == 1 || command == "cl")
		{
			out << "Command " << data[0] << " has userlevel requirement " << bot.permission.GetCommandLevel(data[0]);
			if (bot.commands[data[0]]->AcceptsFlags())
				out << " or requires one of the user flags +" << bot.permission.GetCommandFlags(data[0]);
			out << ".";
		}
		else
		{
			istringstream c(data[1]);
			int newlevel = -2;
			c >> newlevel;
			if (!c || newlevel < -1)
			{
				bot.permission.SetCommandFlags(data[0], data[1]);
				out << "Command " << data[0] << " has been given userflag requirement " << bot.permission.GetCommandFlags(data[0]) << ".";
			}
			else
			{
				out << "Command " << data[0] << " has been given userlevel requirement " << newlevel << ".";
				bot.permission.SetCommandLevel(data[0], newlevel);
			}
		}
		if (out.str() != "")
			bot.Say(respondto, out.str());
	}
	return;
}

vector<string> CmdAllow::CommandStrings()
{
	vector<string> types;
	types.push_back("allow");
	types.push_back("command");
	types.push_back("ul");
	types.push_back("cl");
	return types;
}

string CmdAllow::HelpMsg(string command)
{
	if (command == "allow")
		return "Usage: allow [~]<user> [userlevel] -- Manipulates a user's level of bot access.  Omitting the userlevel field will echo the users current level.  Use the squiggle to force the provided name to be interpreted as the services username, otherwise it is interpreted as the nickname, and the services username will be looked up.";
	else if (command == "command")
		return "Usage: command <command> [userlevel] -- Manipulates the user level requirement of a command.  Omitting the userlevel field will echo the current required level.";
	else if (command == "ul")
		return "Usage: ul <username> -- gets the access level of the specified username.";
	else if (command == "cl")
		return "Usage: cl <command> -- gets the access level requirement of the specified command.";
}
