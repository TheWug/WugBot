#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "CmdIp.h"
#include "DCmd.h"
#include "IRCBot.h"
#include "misc.h"

CmdIP::CmdIP()
{
	defaultuserlevel = 0;
}

vector<string> CmdIP::CommandStrings()
{
	vector<string> types;
	types.push_back("ip");
	types.push_back("host");
	types.push_back("account");
	return types;
}

void CmdIP::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream ch(tolower(args));
	string u;
	ch >> u;
	string t = (u == "") ? speaker.GetNick() : u;
	bot.AddDeferredCommand(new IPReader(command, speaker, target, t, bot));
}

string CmdIP::HelpMsg(string command)
{
	if (command == "ip")
		return "Usage: ip [user] -- Prints the IP address of the specified user.  If no user is specified, your ip will be printed.";
	if (command == "account")
		return "Usage: account [user] -- Prints the account of the specified user.  If no user is specified, your account will be printed.";
	if (command == "host")
		return "Usage: host [user] -- Prints the hostname of the specified user.  If no user is specified, your hostname will be printed.";
}
