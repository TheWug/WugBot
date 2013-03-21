#include <string>

#include "DeferredAccessCheck.h"
#include "IRCBot.h"
#include "misc.h"

using namespace std;

DeferredAccessCheck::DeferredAccessCheck(IRCBot& bot, string cmd, Hostname speaker, string args) : dcmd(speaker, "", bot)
{
	originalcmd = cmd;
	originalmsg = args;
	bot.Whois(speaker.GetNick());
	name = speaker.GetNickL();
	success = false;
}

bool DeferredAccessCheck::Call(string command, Hostname x, string y, string z, string message, bool ctcpflag)
{
	if (command == "318")
	{
		if (tolower(message).find(name) == name.npos)
			return false;
		if (!success)
		{
			originalspeaker.SetAccount("*");
			bot.ProcessFromCmd(originalspeaker, originalcmd, originalmsg);
		}
		return true;
	}
	if (command == "330")
	{
		vector<string> tokens = splitcommands(message, " ", false);
		string subject = tolower(tokens[0]);
		string username = tolower(tokens[1]);
		if (subject != name)
		{
			return false;
		}
		success = true;
		originalspeaker.SetAccount(username);
		bot.ProcessFromCmd(originalspeaker, originalcmd, originalmsg);
	}
	return false;
}
