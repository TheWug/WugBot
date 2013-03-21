#include <iostream>
#include <stdlib.h>

#include "CmdAccess.h"
#include "IgnoreAccessModifier.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"

CmdAccess::CmdAccess()
{
	defaultuserlevel = 4;
}

vector<string> CmdAccess::CommandStrings()
{
	vector<string> types;
	types.push_back("mute");
	types.push_back("block");
	types.push_back("ignore");
	types.push_back("chat");
	return types;
}

void CmdAccess::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream t(args);
	string subcommand;
	t >> subcommand;
	if (command == "mute")
		Mute(bot, speaker, respondto, subcommand, getline(t));
	else if (command == "block")
		Ban(bot, speaker, respondto, subcommand, getline(t));
	else if (command == "ignore")
		Ignore(bot, speaker, target, respondto, subcommand, getline(t));
	else if (command == "chat")
		Chat(bot, speaker, respondto, subcommand, getline(t));
	else
		bot.Say(respondto, "Invalid use of command.");
}

string CmdAccess::HelpMsg(string command)
{
	if (command == "mute")
		return "Usage: mute <+|-> <channel> -- Mutes or unmutes the bot in the specified channel.";
	if (command == "block")
		return "Usage: block <+|-> <channel> -- Prevents the bot from joining the specified channel.";
	if (command == "ignore")
		return "Usage: ignore <+|-> <user> -- prevents the bot from interacting with the specified user.";
	if (command == "chat")
		return "Usage: chat <+|-> <channel> -- Enables or disables random bot chatter in the specified channel.";
}

void CmdAccess::Mute(IRCBot& bot, Hostname& speaker, string respondto, string subcommand, string args)
{
	istringstream t(tolower(args));
	string target;
	t >> target;
	if (subcommand == "add" || subcommand == "+")
	{
		if (args.length() == 0)
			bot.Say(respondto, "You must specify a user or channel.");
		else if (bot.permission.CanSendTo(target))
		{
			bot.permission.MuteToTarget(target);
			bot.Say(respondto, "I will no longer send messages to " + args + ".");
		}
		else
			bot.Say(respondto, "I've already been muted to that target.");
	}
	else if (subcommand == "remove" || subcommand == "-")
	{
		if (args.length() == 0)
			bot.Say(respondto, "You must specify a user or channel.");
		else if (!bot.permission.CanSendTo(target))
		{
			bot.permission.UnmuteToTarget(target);
			bot.Say(respondto, "I will send messages to " + args + " again.");
		}
		else
			bot.Say(respondto, "I'm not muted to that target.");
	}
	else
		bot.Say(respondto, "Invalid subcommand.");
}

void CmdAccess::Ban(IRCBot& bot, Hostname& speaker, string respondto, string subcommand, string args)
{
	istringstream t(tolower(args));
	string chan;
	t >> chan;
	if (subcommand == "add" || subcommand == "+")
	{
		if (args.length() == 0 || args.at(0) != '#')
			bot.Say(respondto, "You must specify a channel.");
		else if (bot.permission.CanBeIn(chan))
		{
			bot.permission.BanFromChannel(chan);
			bot.Say(respondto, "I will no longer enter " + args + ".");
		}
		else
			bot.Say(respondto, "I'm already banned in that channel.");
	}
	else if (subcommand == "remove" || subcommand == "-")
	{
		if (args.length() == 0 || args.at(0) != '#')
			bot.Say(respondto, "You must specify a channel.");
		else if (!bot.permission.CanBeIn(chan))
		{
			bot.permission.UnbanFromChannel(chan);
			bot.Say(respondto, "I will enter " + args + " again.");
		}
		else
			bot.Say(respondto, "I'm not banned in that channel.");
	}
	else
		bot.Say(respondto, "Invalid subcommand.");
}

void CmdAccess::Ignore(IRCBot& bot, Hostname& speaker, string target, string respondto, string subcommand, string args)
{
	istringstream t(args);
	string type, user;
	t >> type;
	t >> user;
	type = tolower(type);
	if (type == "nick" || type == "n" || type == "nickname") type = bot.permission.B_NICK;
	else if (type == "user" || type == "u") type = bot.permission.B_USER;
	else if (type == "account" || type == "acct" || type == "a") type = bot.permission.B_ACCT;
	else if (type == "host" || type == "h") type = bot.permission.B_HOST;
	else if (user != "")
	{
		bot.Say(respondto, "Invalid block criteria: use nick, host, account, or user");
		return;
	}
	else
	{
		user = type;
		type = "";
	}
	if (type != "")
	{
		if (args.length() == 0 || args.at(0) == '#')
			bot.Say(respondto, "You must specify a user.");
		else if (subcommand == "add" || subcommand == "+")
		{
			cout << "Is user banned? " << bot.permission.UserBanned(type, user) << endl;
			if (!bot.permission.UserBanned(type, user))
			{
				bot.permission.BanUser(type, user);
				bot.Say(respondto, type + user + " is no longer allowed to interact.");
			}
			else
				bot.Say(respondto, "That user is already ignored.");
		}
		else if (subcommand == "remove" || subcommand == "-")
		{
			cout << "Is user banned? " << bot.permission.UserBanned(type, user) << endl;
			if (bot.permission.UserBanned(type, user))
			{
				bot.permission.UnbanUser(type, user);
				bot.Say(respondto, type + user + " is now allowed to interact.");
			}
			else
				bot.Say(respondto, "That user is not ignored.");
		}
		else
			bot.Say(respondto, "Invalid subcommand.");
	}
	else
	{
		bot.AddDeferredCommand(new IgnoreAccessModifier(bot, speaker, target, user, subcommand));
	}
}

void CmdAccess::Chat(IRCBot& bot, Hostname& speaker, string respondto, string subcommand, string args)
{
	istringstream t(tolower(args));
	string chan;
	t >> chan;
	if (subcommand == "deny" || subcommand == "-")
	{
		if (args.length() == 0 || args.at(0) != '#')
			bot.Say(respondto, "You must specify a channel.");
		else if (bot.permission.SpamEnabled(chan))
		{
			bot.permission.DenySpam(chan);
			bot.Say(respondto, "General chat responses are no longer allowed in " + chan + ".");
		}
		else
			bot.Say(respondto, "General chat is already disabled.");
	}
	else if (subcommand == "allow" || subcommand == "+")
	{
		if (args.length() == 0 || args.at(0) != '#')
			bot.Say(respondto, "You must specify a channel.");
		else if (!bot.permission.SpamEnabled(chan))
		{
			bot.permission.AllowSpam(chan);
			bot.Say(respondto, "General chat responses are now allowed in " + chan + ".");
		}
		else
			bot.Say(respondto, "General chat is already enabled.");
	}
	else
		bot.Say(respondto, "Invalid subcommand.");
}
