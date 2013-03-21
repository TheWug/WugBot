#include <string>

#include "UserWatcher.h"
#include "IRCBot.h"
#include "misc.h"

using namespace std;

UserWatcher::UserWatcher(IRCBot& bot) : dcmd(Hostname(""), "", bot)
{
}

bool UserWatcher::Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag)
{
	if (command == "quit" || command == "kick")
		return false;
	map<string, char> sent;
	map<string, char>& specific = watchlist[speaker.GetNickL()][tolower(target)];
	map<string, char>& general = watchlist[speaker.GetNickL()]["*"];
	for (map<string, char>::iterator i = specific.begin(); i != specific.end(); i++)
	{
		if (sent[i->first] == 0)
		{
			sent[i->first] = 1;
			bot.Say(i->first, speaker.GetNick() + " appears to be active in " + target + ".");
		}
	}
	watchlist[speaker.GetNickL()].erase(tolower(target));
	for (map<string, char>::iterator i = general.begin(); i != general.end(); i++)
	{
		if (sent[i->first] == 0)
		{
			sent[i->first] = 1;
			bot.Say(i->first, speaker.GetNick() + " appears to be active in " + target + ".");
		}
	}
	watchlist[speaker.GetNickL()].erase("*");
	return false;
}

void UserWatcher::Watch(string target, string user, string location)
{
	((watchlist[user])[location])[target] = 1;
}
