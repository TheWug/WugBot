#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "CmdQuit.h"
#include "IRCBot.h"
#include "misc.h"

CmdQuit::CmdQuit()
{
	defaultuserlevel = 5;
}

void CmdQuit::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	if (args.find("force") == 0) // args starts with "force"
	{
		bot.connected = false;
		istringstream t(args);
		string dummy;
		t >> dummy;
		args = getline(t);
	}
	bot.Quit(args);
	return;
}

vector<string> CmdQuit::CommandStrings()
{
	vector<string> types;
	types.push_back("quit");
	types.push_back("exit");
	types.push_back("disconnect");
	return types;
}

string CmdQuit::HelpMsg(string command)
{
	return "Usage: " + command + " -- Makes the bot quit.  The bot's program will exit.";
}
