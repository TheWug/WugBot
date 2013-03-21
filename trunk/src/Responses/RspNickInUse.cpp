#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "RspNickInUse.h"

#include "IRCBot.h"
#include "misc.h"

using namespace std;

void RspNickInUse::PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	cout << message << endl;
	cout << "Trying again with " << bot.altnick << "..." << endl;
	if (bot.registered == true)
		return;
	bot.Nick(bot.altnick);
	bot.nick = bot.altnick;
	bot.lnick = tolower(bot.nick);
	bot.altnick += '_';
}

string RspNickInUse::RespondTo()
{
	return "433";
}
