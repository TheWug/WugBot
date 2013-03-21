#include <iostream>

#include "MonitorWatcher.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"

MonitorWatcher::MonitorWatcher(IRCBot& bot, map<string, set<string> >& m) : dcmd(Hostname(""), "", bot), monitored(m)
{
}

MonitorWatcher::~MonitorWatcher()
{
}

bool MonitorWatcher::Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag)
{
	if (command == "730")
	{
		Hostname h(message);
		map<string, set<string> >::iterator notifies = monitored.find(h.GetNickL());
		if (notifies == monitored.end())
			return false;
		for (set<string>::iterator t = notifies->second.begin(); t != notifies->second.end(); ++t)
		{
			if (bot.permission.CanSendTo(*t))
				bot.Say(*t, h.GetNick() + " has appeared online.  (Hostname: " + h.GetHost() + ")");
		}
	}
	else if (command == "731")
	{
		map<string, set<string> >::iterator notifies = monitored.find(tolower(message));
		if (notifies == monitored.end())
			return false;
		for (set<string>::iterator t = notifies->second.begin(); t != notifies->second.end(); ++t)
		{
			if (bot.permission.CanSendTo(*t))
				bot.Say(*t, message + " is no longer online.");
		}
	}
	return false;
}
