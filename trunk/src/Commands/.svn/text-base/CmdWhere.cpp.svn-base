#include "CmdWhere.h"
#include "IRCBot.h"

CmdWhere::CmdWhere()
{
	defaultuserlevel = 1;
}

void CmdWhere::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	set<string>::iterator end = bot.channellist.end();
	string message = "";
	for (set<string>::iterator i = bot.channellist.begin(); i != end; )
	{
		message += " " + *i;
		if (++i != end)
			message += ",";
	}
	if (message == "")
		message += "I am not on any channels.";
	else
		message = "I am in the following channels:" + message + ".";
	bot.Say(respondto, message);
}

string CmdWhere::HelpMsg(string command)
{
	return "Usage: where -- Displays a list of every channel the bot is in.";
}

vector<string> CmdWhere::CommandStrings()
{
	vector<string> types;
	types.push_back("where");
	return types;
}

