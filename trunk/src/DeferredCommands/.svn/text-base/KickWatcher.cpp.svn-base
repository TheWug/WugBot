#include <string>

#include "KickWatcher.h"
#include "IRCBot.h"
#include "misc.h"

using namespace std;

KickWatcher::KickWatcher(string subj, string chan, Hostname speaker, string target, IRCBot& bot, string message) : dcmd(speaker, target, bot)
{
	bot.Kick(subj, chan, message);
	subject = subj;
	channel = chan;
}

bool KickWatcher::Call(string command, Hostname s, string t, string respondto, string message, bool ctcpflag)
{
	istringstream ch(tolower(message));
	string u;
	ch >> u;
	if (s.GetNickL() == bot.lnick && channel == t && command == "kick" && u == tolower(subject))
	{
		return true;
	}
	else if (tolower(t) == bot.lnick && u == channel && command == "482" )
	{
		bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "I don't have operator privilages in " + channel + ".");
		return true;
	}
	else if (tolower(t) == bot.lnick && command == "461" )
	{
		bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "Not enough parameters.");
		return true;
	}
	else if (tolower(t) == bot.lnick && command == "403" )
	{
		bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "Channel " + channel + " does not exist.");
		return true;
	}
	else if (tolower(t) == bot.lnick && command == "401" )
	{
		bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), subject + " is not in the channel.");
		return true;
	}
	else if (tolower(t) == bot.lnick && u == channel && command == "442" )
	{
		bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "I'm not on " + channel + ".");
		return true;
	}
	return false;
}
