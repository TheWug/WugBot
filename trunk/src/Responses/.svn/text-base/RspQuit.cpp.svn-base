#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "RspQuit.h"

#include "IRCBot.h"
#include "UserManager.h"
#include "misc.h"

using namespace std;

void RspQuit::PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
}

void RspQuit::PostAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	bot.usermgr.UserLeftAllChannels(target, speaker.GetNick());
	cout << speaker.GetNick() << " has disconnected " << " (" << message.c_str() << ")" << endl;
}

string RspQuit::RespondTo()
{
	return "quit";
}
