#include <string>

#include "IgnoreAccessModifier.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"

using namespace std;

IgnoreAccessModifier::IgnoreAccessModifier(IRCBot& bot, Hostname speaker, string target, string user, string cmd) : dcmd(speaker, target, bot)
{
	targetuser = user;
	subcommand = cmd;
	success = false;
	userdata = Hostname("");
	cout << "whoing: " << user << endl;
	bot.Who(user, "%uhna");
}

bool IgnoreAccessModifier::Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag)
{
	if (command == "263")
	{
		bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "Could not complete request due to rate limiting, try again in a second.");
		return true;
	}
	if (command == "315")
	{
		if (tolower(message).find(tolower(targetuser)) == targetuser.npos)
			return false;
		if (success)
		{
			if (subcommand == "add" || subcommand == "+")
			{
				bot.permission.BanUser(userdata);
				bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), userdata.GetNick() + " is no longer allowed to interact.");
			}
			else if (subcommand == "remove" || subcommand == "-")
			{
				bot.permission.UnbanUser(userdata);
				bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), userdata.GetNick() + " is now allowed to interact.");
			}
		}
		else
		{
			bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "Unable to find user \"" + targetuser + "\".  Perhaps you meant to ban a nickname with ;ignore add nickname <nick>?");
		}
		return true;
	}
	if (command == "354")
	{
		cout << "got 354" << endl;
		vector<string> tokens = splitcommands(message, " ", false);
		if (tokens.size() != 4)
			return false;
		string nick = tokens[2];
		if (tolower(nick) != tolower(targetuser))
			return false;
		userdata.SetNick(nick);
		userdata.SetHost(tokens[1]);
		userdata.SetUser(tokens[0]);
		userdata.SetAccount(tokens[3]);
		success = true;
	}
	return false;
}
