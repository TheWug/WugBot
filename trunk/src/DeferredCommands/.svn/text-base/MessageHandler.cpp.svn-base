#include <string>

#include "MessageHandler.h"
#include "IRCBot.h"
#include "misc.h"

using namespace std;

bool MessageHandler::Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag)
{
	map<string, map<string, map<string, string> > >::iterator f = watchlist.find(command);
	if (f == watchlist.end())
	{
		return false;
	}
	map<string, map<string, string> >::iterator g;
	if (tolower(target) == bot.lnick)
		g = f->second.find("@");
	else		
		g = f->second.find(tolower(target));
	if (g == f->second.end())
	{
		g = f->second.find("*");
		if (g == f->second.end())
		{
			return false;
		}
	}
	map<string, string>::iterator h = g->second.find(tolower(speaker.GetNickL()));
	if (h == g->second.end())
	{
		h = g->second.find("*");
		if (h == g->second.end())
		{
			return false;
		}
	}
	bot.PerformCommand(speaker, target, respondto, Replace(speaker, target, message, h->second));
	return false;
}

void MessageHandler::SetCommand(string message, string target, string subject, string command)
{
	((watchlist[message])[target])[subject] = command;
}

void MessageHandler::ClearCommand(string message, string target, string subject)
{
	(watchlist[message])[target].erase(subject);
	if ((watchlist[message])[target].size() == 0)
		(watchlist[message])[target].erase(message);
	if (watchlist[message].size() == 0)
		watchlist[message].erase(target);
}

string MessageHandler::Replace(Hostname speaker, string target, string message, string command)
{
	return stringreplace(stringreplace(stringreplace(stringreplace(command, "%n", speaker.GetNick()), "%h", speaker.GetHost()), "%t", target), "%m", message);
}
