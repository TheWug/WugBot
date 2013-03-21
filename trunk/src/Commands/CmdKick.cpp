#include <iostream>
#include <stdlib.h>

#include "CmdKick.h"
#include "KickWatcher.h"
#include "IRCBot.h"
#include "misc.h"

CmdKick::CmdKick()
{
	defaultuserlevel = 2;
	defaultuserflags = "oOcC";
	acceptsflags = true;
}

vector<string> CmdKick::CommandStrings()
{
	vector<string> types;
	types.push_back("kick");
	types.push_back("k");
	return types;
}

void CmdKick::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream t(args);
	string user = "", channel = tolower(target), message = "";
	t >> user;
	user = tolower(user);
	string rem = getline(t);
	istringstream t2(rem);
	if (rem.length() != 0 && rem.at(0) == '#')
	{
		t2 >> channel;
		channel = tolower(channel);
	}
	if (channel.length() == 0 || channel.at(0) != '#')
		bot.Say(respondto, "You must specify a valid channel to kick from.");
	else if (user.length() == 0)
		bot.Say(respondto, "You must specify a valid user to kick.");
	else if (bot.channellist.find(channel) == bot.channellist.end())
		bot.Say(respondto, "I'm not in that channel.");
	else
	{
		message = getline(t2);
		bot.AddDeferredCommand(new KickWatcher(user, channel, speaker, target, bot, message));
	}
}

void CmdKick::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream t(args);
	string user = "", channel = tolower(target);
	t >> user;
	user = tolower(user);
	string rem = getline(t);
	istringstream t2(rem);
	if (rem.length() != 0 && rem.at(0) == '#')
	{
		t2 >> channel;
		channel = tolower(channel);
	}
	FlagTestAndGo(bot, command, speaker, target, respondto, args, channel);
}
	

string CmdKick::HelpMsg(string command)
{
	return "Usage: kick <user> [channel [reason]] -- Kicks the specified user from the specified channel, with the specified (optional) reason.  If no channel is specified, the current one will be used.  The bot must have operator privilages to kick.";
}
