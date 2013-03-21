#include <string>

#include "ModeWatcher.h"
#include "IRCBot.h"
#include "misc.h"

using namespace std;

ModeWatcher::ModeWatcher(IRCBot& bot, Hostname speaker, string target, string nmode, string nuser, string nchannel) : dcmd(speaker, target, bot)
{
	mode = nmode; user = nuser; channel = nchannel;
	bot.Mode(channel, mode + " " + user);
}
	
bool ModeWatcher::Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag)
{
	istringstream args(tolower(message));
	string targetchan;
	args >> targetchan;

	if (tolower(targetchan) != tolower(channel))
		return false;
	
	if (command == "482")
	{
		bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "I'm not an operator on that channel.");
		return true;
	}
	else if (command == "403")
	{
		bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "That channel doesnt exist.");
		return true;
	}
	else if (command == "mode")
	{
		return true;
	}
}
