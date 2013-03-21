#include <iostream>
#include <stdlib.h>
#include <fstream>

#include "CmdHaspaid.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"
#include "HTTP.h"

#include "misc.h"

CmdHaspaid::CmdHaspaid()
{
	defaultuserlevel = 0;
}

vector<string> CmdHaspaid::CommandStrings()
{
	vector<string> types;
	types.push_back("paid");
	types.push_back("haspaid");
	types.push_back("hp");
	types.push_back("hp-url");
	return types;
}

void CmdHaspaid::PostInstall(IRCBot& bot)
{
	bot.permission.SetDefaultCommandLevel("hp-url", 2);
	ifstream pfile("IRCBot.haspaid");
	getline(pfile, haspaidURL, '\n');
	pfile.close();
}

void CmdHaspaid::SaveState()
{
	ofstream pfile("IRCBot.haspaid");
	pfile << haspaidURL << endl;
	pfile.close();
}

void CmdHaspaid::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	if (command == "hp-url")
	{
		if (args == "")
		{
			bot.Say(respondto, "Haspaid URL is currently \"" + haspaidURL + "\".");
		}
		else
		{
			haspaidURL = args;
			bot.Say(respondto, "Haspaid URL set to \"" + haspaidURL + "\".");
		}
		return;
	}
	string user, type;
	istringstream s(args);

	target = respondto;
	type = "privmsg";

	s >> user;
	s >> target;
	s >> type;

	string response;
	try
	{
		if (HasPaid(user))
			response = user + " has paid for minecraft.";
		else
			response = user + " has NOT paid for minecraft.";
	}
	catch (string s)
	{
		response = "Error: " + s;
	}
	if (tolower(type) == "notice")
		bot.Notice(target, response);
	else
		bot.Message(target, response);
}

string CmdHaspaid::GetURL(string user)
{
	return stringreplace(haspaidURL, "%s", user);
}

bool CmdHaspaid::HasPaid(string user) throw (string)
{
	HTTPRequest req;
	req.SetTarget(GetURL(user));
	//req.host = "www.minecraft.net";
	//req.server = "www.minecraft.net";
	//req.page = "/haspaid.jsp";
	//req.get["user"] = user;
	string result;
	int rv = req.Do();

	//cout << "\"" << req.GetHeader() << "\"" << endl;
	//cout << "\"" << req.GetResponse() << "\"" << endl;
	//cout << req.GetCode() << endl;

	if (rv >= 200 && rv < 300)
		return (req.GetResponse() == "true");
	else if (rv == -5)
		throw string("Could not resolve minecraft.net");
	else if (rv == -1)
		throw string("FCNTL error (this should never happen)");
	else if (rv == -3)
		throw string("Could not connect to minecraft.net");
	else if (rv == -2)
		throw string("Request timed out");
	else if (rv > 0)
		throw string("Invalid HTTP response: ") + tostr(rv);
	return false;
}

string CmdHaspaid::HelpMsg(string command)
{
	return  (command != "hp-url") ?
			"Usage: " + command + " <minecraft username> [respondto] [responsetype] -- Checks whether a minecraft account has premium status or not.  If respondto and respondtype are specified, the bot will respond send a message of type respondtype to respondto.\nrespondtype defaults to privmsg and can be either privmsg or notice, respondto defaults to the channel in which the command was used (or the user, if used via pm)."
		:
			"Usage: " + command + " <new url> -- Assigns the URL that haspaid will query.  Normally, this should be set to http://minecraft.net/haspaid.jsp?user=%s (the minecraft haspaid service).  Use %s to represent the username.";
}
