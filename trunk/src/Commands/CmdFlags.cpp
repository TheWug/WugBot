#include <iostream>
#include <sstream>

#include "CmdFlags.h"
#include "UserManager.h"
#include "DeferredFlags.h"
#include "IRCBot.h"
#include "misc.h"

CmdFlags::CmdFlags()
{
	defaultuserlevel = 5;
	defaultuserflags = "Ss";
	acceptsflags = true;
}

vector<string> CmdFlags::CommandStrings()
{
	vector<string> types;
	types.push_back("flags");
	types.push_back("f");
	return types;
}

void CmdFlags::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	string channel = target;
	string user = speaker.GetNickL();
	string flags = "";
	istringstream temp(args);
	string dummy;
	temp >> dummy;
	if (dummy.length() != 0 && dummy.at(0) == '#')
	{
		channel = dummy;
		dummy = "";
		temp >> dummy;
	}
	if (dummy.length() != 0 && (dummy.at(0) == '+' || dummy.at(0) == '-'))
	{
		flags = dummy;
		dummy = "";
		temp >> dummy;
	}
	else if (dummy.length() != 0)
	{
		user = dummy;
		dummy = "";
		temp >> dummy;
		if (dummy != "")
			flags = dummy;
	}
	if (channel.length() == 0 || channel.at(0) != '#')
	{
		bot.Say(respondto, "You must specify a valid channel.");
		return;
	}
	string h;
	if (user.at(0) == '~')
		h = tolower(user.substr(1));
	else
	{
		h = bot.usermgr.GetUsernameFromNick(user);
		if (h == "")
		{
			bot.AddDeferredCommand(new DeferredFlags(speaker, target, bot, channel, user, flags));
			return;
		}
	}
	if (flags == "")
	{
		bot.Say(respondto, h + " has (in " + channel + ") flags: " + bot.usermgr.UserFlagString(tolower(channel), tolower(h)));
		return;
	}
	else
	{
		string luser = tolower(h);
		string lchan = tolower(channel);
		bool set = true;
		for (string::iterator i = flags.begin(); i != flags.end(); i++)
		{
			if (*i == '+')
				set = true;
			else if (*i == '-')
				set = false;
			else
			{
				if (IsAlphaNumeric(*i))
					bot.usermgr.SetChannelUFlag(lchan, luser, set, *i);
			}
		}
		bot.Say(respondto, h + " (in " + channel + ") has been given flags: " + bot.usermgr.UserFlagString(lchan, luser));
	}
}

void CmdFlags::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	string channel = target, dummy;
	istringstream temp(args);
	temp >> dummy;
	if (dummy.length() != 0 && dummy.at(0) == '#')
		channel = dummy;
	FlagTestAndGo(bot, command, speaker, target, respondto, args, channel);
}

string CmdFlags::HelpMsg(string command)
{
	return "Usage: flags [channel] [user] [flags] -- prints or sets the flags for specified user in specified channel.  If channel is omitted, the current channel is used.  If user is omitted, the current user is used.  If flags are omitted, the flags are displayed rather than set.\nThe flags of a user may be any of the following characters: " + availableflags;
}
