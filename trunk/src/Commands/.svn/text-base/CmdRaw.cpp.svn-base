#include <iostream>
#include <stdlib.h>

#include "CmdRaw.h"
#include "IRCBot.h"

CmdRaw::CmdRaw()
{
	defaultuserlevel = 4;
}

vector<string> CmdRaw::CommandStrings()
{
	vector<string> types;
	types.push_back("raw");
	return types;
}

void CmdRaw::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	bot.Raw(args);
}

string CmdRaw::HelpMsg(string command)
{
	return "Usage: raw <string> -- sends the provided string directly to the server.  Can be used to mimic features that I don't natively have, or bypass access restrictions.  Beware.";
}
