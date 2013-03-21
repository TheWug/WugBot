#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "RspJoin.h"

#include "IRCBot.h"
#include "UserManager.h"
#include "misc.h"

using namespace std;

void RspJoin::PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	if (target.at(0) == ':')
		target = target.substr(1);
	cout << speaker.GetNick() << " has joined " << target << endl;
	istringstream resp(message);
	string username;
	resp >> username;
	if (username == "")
		username = "*";
	bot.usermgr.UserJoinedChannel(target, speaker.GetNick());
	bot.usermgr.AddUser(speaker.GetNick(), speaker.GetUser(), speaker.GetHost(), username);
	if (speaker.GetNick() == bot.nick)
		bot.OnChannelEnter(target);
}

string RspJoin::RespondTo()
{
	return "join";
}
