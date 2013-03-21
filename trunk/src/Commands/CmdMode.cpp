#include <iostream>
#include <stdlib.h>

#include "CmdMode.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"

vector<string> CmdMode::CommandStrings()
{
	vector<string> types;
	types.push_back("mode");
	types.push_back("m");
	types.push_back("updatemodes");
	types.push_back("listmodes");
	return types;
}

CmdMode::CmdMode()
{
	modes = "ntspmircgzLPFQCfjklI";
	defaultuserlevel = 3;
	defaultuserflags = "oOm";
	acceptsflags = true;
}

void CmdMode::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	if (command == "listmodes")
	{
		bot.Say(respondto, "Modes: " + modes);
	}
	if (command == "updatemodes")
	{
		bot.Say(respondto, "Setting list of available modes to: " + args);
		modes = "+-" + args;
	}
	else
	{
		string channel = target;
		if (args.length() != 0 && args.at(0) == '#')
		{
			istringstream ch(args);
			ch >> channel;
			args = getline(ch);
		}
		if (channel.length() == 0 || channel.at(0) != '#')
		{
			bot.Say(respondto, "You must pick a channel to set modes in.");
			return;
		}
		int i = 0;
		string newmodes;
		for (; args.length() != i && args.at(i) != ' '; i++)
		{
			if ((modes.find(args.at(i)) != args.npos && newmodes.find(args.at(i)) == args.npos) || args.at(i) == '+' || args.at(i) == '-')
				newmodes.push_back(args.at(i));
		}
		string newargs = "";
		if (i != args.length())
			newargs = args.substr(i);
		bot.Mode(channel, newmodes + " " + newargs);
	}
}

void CmdMode::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	if (command != "mode")
		return;
	string channel = target;
	if (args.length() != 0 && args.at(0) == '#')
	{
		istringstream ch(args);
		ch >> channel;
	}
	FlagTestAndGo(bot, command, speaker, target, respondto, args, channel);
}
	

string CmdMode::HelpMsg(string command)
{
	if (command == "mode" || command == "m")
		return "Usage: mode [channel] <mode> [args]  -- sets provided mode on provided channel.  If the channel isnt specified, the current one is used.  Will fail if the bot does not have operator privilages.  Provided mode can include setting and clearing multiple modes at once, but should not be broken up by spaces.";
	if (command == "updatemodes")
		return "Usage: updatemodes <modelist> -- sets the list of channel modes that can be set with this command.";
}

void CmdMode::PostInstall(IRCBot& bot)
{
	string s = "updatemodes";
	bot.permission.SetCommandLevel(s, 4);
}
