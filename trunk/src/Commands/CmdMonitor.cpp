#include <iostream>
#include <sstream>

#include "CmdMonitor.h"

#include "MonitorWatcher.h"
#include "BotPermissions.h"
#include "misc.h"
#include "IRCBot.h"
#include "IRC.h"

CmdMonitor::CmdMonitor(IRCBot& bot)
{
	defaultuserlevel = 0;
	monitorwatcher = new MonitorWatcher(bot, monitored);
	bot.AddDeferredCommand(monitorwatcher);
}

CmdMonitor::~CmdMonitor()
{
	delete monitorwatcher;
}

vector<string> CmdMonitor::CommandStrings()
{
	vector<string> types;
	types.push_back("monitor");
	types.push_back("unmonitor");
	types.push_back("pmonitor");
	types.push_back("unpmonitor");
	types.push_back("monitor-clear");
	return types;
}

void CmdMonitor::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream s(args);
	string mcommand, mtarget;
	s >> mcommand; mcommand = tolower(mcommand);
	s >> mtarget;  mtarget = tolower(mtarget);

	if (mtarget == "")
	{
		mtarget = mcommand;
		mcommand = speaker.GetNickL();
	}

	if (command == "monitor-do");
	else if (command == "unmonitor-do");
	else if (command == "monitor-clear")
	{
		monitored.clear();
		bot.Monitor("C", "");
		bot.Say(respondto, "Monitor list has been cleared.");
	}
	else if (command == "monitor")
		AddMonitorEntry(bot, speaker, respondto, mtarget, speaker.GetNickL());
	else if (command == "unmonitor")
		RemoveMonitorEntry(bot, speaker, respondto, mtarget, speaker.GetNickL());
	else if (command == "pmonitor")
		AddMonitorEntry(bot, speaker, respondto, mtarget, mcommand);
	else if (command == "unpmonitor")
		RemoveMonitorEntry(bot, speaker, respondto, mtarget, mcommand);
}

void CmdMonitor::AddMonitorEntry(IRCBot& bot, Hostname speaker, string respondto, string mtarget, string watcher)
{
	if (!IRC::IsValidNick(bot.serverproperties, mtarget) && !IRC::IsValidChannel(bot.serverproperties, mtarget))
	{
		bot.Say(respondto, "You must specify a valid nickname.");
		return;
	}

	map<string, set<string> >::iterator monitorentry = monitored.find(mtarget);
	if (monitorentry != monitored.end())
	{
		monitorentry->second.insert(watcher);
		bot.Say(respondto, "If " + mtarget + " appears online I'll be sure to tell you.");
	}
	else
	{
		if (monitored.size() >= 100)
			bot.Say(respondto, "I cannot monitor any more nicknames.");
		else
		{
			monitored[mtarget].insert(watcher);
			bot.Monitor("+", mtarget);
			bot.Say(respondto, "If " + mtarget + " appears online I'll be sure to tell you.");
		}
	}
}

void CmdMonitor::RemoveMonitorEntry(IRCBot& bot, Hostname speaker, string respondto, string mtarget, string watcher)
{
	if (!IRC::IsValidNick(bot.serverproperties, mtarget) && !IRC::IsValidChannel(bot.serverproperties, mtarget))
	{
		bot.Say(respondto, "You must specify a valid nickname.");
		return;
	}

	map<string, set<string> >::iterator monitorentry = monitored.find(mtarget);
	if (monitorentry == monitored.end())
	{
		bot.Say(respondto, "You are not monitoring that nickname.");
	}
	else
	{
		set<string>& entry = monitored[mtarget];
		if (entry.find(watcher) == entry.end())
			bot.Say(respondto, "You are not monitoring that nickname.");
		else
		{
			entry.erase(watcher);
			if (entry.size() == 0)
			{
				bot.Monitor("-", mtarget);
				monitored.erase(mtarget);
			}
			bot.Say(respondto, " I'll stop alerting you about " + mtarget + ".");
		}
	}
}

string CmdMonitor::HelpMsg(string command)
{
	if (command == "monitor")
		return "Usage: monitor <nickname> -- Sets you to be notified when the target nickname appears or disappears.  If the target is online when you use the command, you will immediately get an online notice.";
	else if (command == "unmonitor")
		return "Usage: unmonitor <nickname> -- Stops you from being notified when the target nickname appears or disappears.";
	else if (command == "pmonitor")
		return "Usage: pmonitor <target> <nickname> -- Sets provided target to be notified when the target nickname appears or disappears.  If the target is online when you use the command, you will immediately get an online notice.";
	else if (command == "unpmonitor")
		return "Usage: unpmonitor <target> <nickname> -- Stops provided target from being notified when the target nickname appears or disappears.";
	else if (command == "monitor-clear")
		return "Usage: monitor-clear -- clears all stored monitors.  No notifications will be sent.";;
}

void CmdMonitor::PostInstall(IRCBot& bot)
{
	bot.permission.SetDefaultCommandLevel("pmonitor", 2);
	bot.permission.SetDefaultCommandLevel("unpmonitor", 2);
	bot.permission.SetDefaultCommandLevel("monitor-clear", 2);
}

void CmdMonitor::SaveState()
{
}
