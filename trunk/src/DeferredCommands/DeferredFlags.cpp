#include <string>

#include "DeferredFlags.h"
#include "IRCBot.h"
#include "misc.h"
#include "cmd.h"

using namespace std;

DeferredFlags::DeferredFlags(Hostname speaker, string target, IRCBot& bot, string c, string n, string f) : dcmd(speaker, target, bot)
{
	channel = c;
	name = tolower(n);
	flags = f;
	bot.Whois(name);
}

bool DeferredFlags::Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag)
{
	if (command == "330")
	{
		vector<string> tokens = splitcommands(message, " ", false);
		if (tokens.size() < 2)
		{
			bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "Soz, server borked.  try again.");
			return true;
		}
		string subject = tolower(tokens[0]);
		string username = tolower(tokens[1]);
		if (subject == name)
		{
			map<string, cmd *>::iterator found = bot.commands.find("flags");
			(found->second)->ProcessCommand(bot, "flags", originalspeaker, originaltarget, bot.GetRespondTo(originalspeaker, originaltarget), channel + " " + string("~") + username + " " + flags);
			return true;
		}
	}
	else if (command == "318")
	{
		bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "Nobody is logged on with that nickname.");
		return true;
	}
	return false;
}
