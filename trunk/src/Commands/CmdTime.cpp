#include "CmdTime.h"

#include "IRCBot.h"
#include "Bottime.h"
#include "misc.h"

CmdTime::CmdTime()
{
	defaultuserlevel = 0;
}

vector<string> CmdTime::CommandStrings()
{
	vector<string> types;
	types.push_back("uptime");
	types.push_back("time");
	return types;
}

void CmdTime::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	if (command == "uptime")
		bot.Say(respondto, "Bot uptime is: " + GetUptime() + ".");
	else if (command == "time")
		bot.Say(respondto, "System time is: " + GetSystemTime() + ".");
}

string CmdTime::HelpMsg(string command)
{
	if (command == "time")
		return "Usage: time -- prints the system time.";
	if (command == "uptime")
		return "Usage: uptime -- prints the bots uptime.";
}

string CmdTime::GetUptime()
{
	return AsHumanReadableTime(Bottime::GetUptime());
}

string CmdTime::GetSystemTime()
{
	return "UNIMPLEMENTED";
}

