#include <string>

#include "Forwarder.h"
#include "IRCBot.h"
#include "misc.h"

using namespace std;

Forwarder::Forwarder(string newsubj, string newtar, IRCBot& bot) : dcmd(Hostname(""), "", bot)
{
	subject = newsubj;
	target = newtar;

	channel = (subject.length() > 0 && subject.at(0) == '#');
}

bool Forwarder::Call(string command, Hostname speaker, string t, string respondto, string message, bool ctcpflag)
{
	if (channel)
	{
		if (t == subject)
			bot.Say(target, t + "/" + speaker.GetNick() + ": " + message);
	}
	else
	{
		if (speaker.GetNickL() == subject && t == bot.lnick)
			bot.Say(target, speaker.GetNick() + ": " + message);
	}
	return false;
}

string Forwarder::GetSubject()
{
	return subject;
}

string Forwarder::GetTarget()
{
	return target;
}
