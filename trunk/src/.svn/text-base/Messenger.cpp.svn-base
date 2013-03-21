#include "Messenger.h"
#include "IRCBot.h"
#include "BotPermissions.h"

Messenger::Messenger(IRCBot& b, string t, bool m) : bot(b)
{
	notmute = !m;
	target = t;
}

void Messenger::Say(string message)
{
	if (notmute)
		bot.Say(target, message);
}

void Messenger::Message(string message)
{
	if (notmute)
		bot.Message(target, message);
}

void Messenger::Notice(string message)
{
	if (notmute)
		bot.Notice(target, message);
}

void Messenger::Raw(string message)
{
	if (notmute)
		bot.Raw(message);
}

string Messenger::GetTarget()
{
	return target;
}

