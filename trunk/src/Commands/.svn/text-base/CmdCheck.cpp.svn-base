#include <iostream>
#include <stdlib.h>

#include "CmdCheck.h"
#include "ServerPinger.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "ServerPingThread.h"
#include "misc.h"

CmdCheck::CmdCheck()
{
	defaultuserlevel = 0;
}

vector<string> CmdCheck::CommandStrings()
{
	vector<string> types;
	types.push_back("check");
	types.push_back("mcping");
	return types;
}

void CmdCheck::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	args = trim(args);
	if (args == "")
	{
		Dispatch(bot, speaker, target, speaker.GetHost());
		//bot.Say(respondto, "checking " + speaker.GetHost());
	}
	else if (IsNumeric(args))
	{
		Dispatch(bot, speaker, target, speaker.GetHost() + ":" + args);
		//bot.Say(respondto, "checking " + speaker.GetHost() + ":" + args);
	}
	else if (args.find('.') != string::npos)
	{
		Dispatch(bot, speaker, target, args);
		//bot.Say(respondto, "checking " + args);
	}
	else
	{
		// this option also handles calls of the format "nickname:port"
		bot.AddDeferredCommand(new ServerPinger(bot, speaker, target, args));
		//bot.Say(respondto, "checking " + args + "'s host");
	}
	return;
}

string CmdCheck::HelpMsg(string command)
{
	return "Usage: " + command + " [URL | nickname] -- Tests if there is a minecraft (or other kind of) server running from the target.  If a nickname is used, it will test that user's IP address.";
}

void CmdCheck::Dispatch(IRCBot& bot, Hostname speaker, string target, string server)
{
	(new ServerPingThread(bot, speaker, target, server))->start();
}
