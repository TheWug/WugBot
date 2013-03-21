#include <string>

#include "DeferredAllow.h"
#include "IRCBot.h"
#include "misc.h"
#include "cmd.h"

using namespace std;

DeferredAllow::DeferredAllow(Hostname speaker, string target, IRCBot& bot, string args, string c) : dcmd(speaker, target, bot)
{
	istringstream t(args);
	t >> name;
	name = tolower(name);
	bot.Whois(name);
	level = getline(t);
	founduser = false;
	calledby = c;
}

bool DeferredAllow::Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag)
{
	vector<string> tokens = splitcommands(message, " ", false);
	if (tokens.size() < 2) return false;
	string subject = tolower(tokens[0]);
	string username = tolower(tokens[1]);

	if (command == "330" && subject == name)
	{
		map<string, cmd *>::iterator found = bot.commands.find(calledby);
		(found->second)->ProcessCommand(bot, calledby, originalspeaker, originaltarget, bot.GetRespondTo(originalspeaker, originaltarget), string("~") + username + " " + level);
		return true;
	}
	else if (command == "311" && subject == name)
		founduser = true;
	else if (command == "318" && subject == name)
	{
		if (founduser && calledby == "allow")
			bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "User is not logged in.  To set permissions for unauthenticated users, use 'allow *'.");
		else if (founduser && calledby == "ul")
		{
			map<string, cmd *>::iterator found = bot.commands.find(calledby);
			(found->second)->ProcessCommand(bot, calledby, originalspeaker, originaltarget, bot.GetRespondTo(originalspeaker, originaltarget), "*");
			return true;
		}
		else
			bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "Nobody is logged on with that nickname.");
		return true;
	}
	return false;
}
