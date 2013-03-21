#include <sstream>

#include "CmdTopic.h"
#include "IRCBot.h"
#include "misc.h"

CmdTopic::CmdTopic()
{
	defaultuserlevel = 2;
	defaultuserflags = "oOm";
	acceptsflags = true;
}

void CmdTopic::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	string channel = target;
	if (args.length() > 0 && args.at(0) == '#')
	{
		istringstream x(args);
		x >> channel;
		args = getline(x);
	}
	if (channel.length() < 0 || channel.at(0) != '#')
	{
		bot.Say(respondto, "You can only set the topic of channels.");
		return;
	}
	else if (command == "topic")
	{
		bot.Topic(channel, args);
	}
	return;
}

void CmdTopic::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	string channel = target;
	if (args.length() > 0 && args.at(0) == '#')
	{
		istringstream x(args);
		x >> channel;
	}
	FlagTestAndGo(bot, command, speaker, target, respondto, args, channel);
}

string CmdTopic::HelpMsg(string command)
{
	return "Usage: topic [channel] [newtopic] -- sets the topic of specified channel to specified topic.  If channel is not specified, the current channel is used.  If no new topic is specified, the topic is cleared.  The bot must have operator privilages in the specified channel.";
}

vector<string> CmdTopic::CommandStrings()
{
	vector<string> types;
	types.push_back("topic");
	return types;
}

