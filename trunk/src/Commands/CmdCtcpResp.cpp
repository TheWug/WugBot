#include <fstream>
#include <sstream>

#include "CmdCtcpResp.h"
#include "IRCBot.h"
#include "misc.h"

CmdCtcpResp::CmdCtcpResp(IRCBot& b) : mybot(b)
{
	defaultuserlevel = 4;
}

void CmdCtcpResp::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream i(args);
	string c;
	i >> c;
	c = toupper(c);
	args = trim(getline(i));
	if (args == "")
	{
		map<string, string>::iterator i = bot.ctcpresponses.find(c);
		if (i == bot.ctcpresponses.end())
			bot.Say(respondto, "CTCP response for " + c + " is unset.");
		else
			bot.Say(respondto, "CTCP response for " + c + " is: " + i->second);
	}
	else if (args == "DELETE-RESPONSE")
	{
		bot.Say(respondto, "CTCP response for " + c + " deleted.");
		bot.ctcpresponses.erase(c);
	}
	else
	{
		bot.Say(respondto, "CTCP response for " + c + " set to: " + args);
		bot.ctcpresponses[c] = args;
	}
}

string CmdCtcpResp::HelpMsg(string command)
{
	return "Usage: ctcp-resp <CTCP message> <response> -- Sets the bot's response to a ctcp event.  If response is blank, the current response is printed.";
}

vector<string> CmdCtcpResp::CommandStrings()
{
	vector<string> types;
	types.push_back("ctcp-resp");
	return types;
}

void CmdCtcpResp::PostInstall(IRCBot& bot)
{
	ifstream pfile("IRCBot.ctcp");
	int maxsize = 512;
	string ctcp, reply;
	char buffer[maxsize];
	while (true)
	{
		pfile >> ctcp;
		ctcp = toupper(ctcp);
		pfile.getline(buffer, maxsize);
		reply = trim(buffer);
		if (!pfile)
			break;
		bot.ctcpresponses[ctcp] = reply;
	}
	pfile.close();
}

void CmdCtcpResp::SaveState()
{
	ofstream pfile("IRCBot.ctcp");
	for (map<string, string>::iterator i = mybot.ctcpresponses.begin(); i != mybot.ctcpresponses.end(); i++)
	{
		pfile << i->first << " " << i->second << endl;
	}
	pfile.close();
}
