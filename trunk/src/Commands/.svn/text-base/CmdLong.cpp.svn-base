#include <sstream>
#include <fstream>

#include "CmdLong.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "HTTP.h"
#include "misc.h"

CmdLong::CmdLong()
{
	defaultuserlevel = 0;
}

void CmdLong::PostInstall(IRCBot& bot)
{
	ifstream pfile("IRCBot.unshorten");
	string token;

	while (true)
	{
		pfile >> token;
		if (!pfile) break;

		if (token == "[ApiKey]")
		{
			getline(pfile, token, '\n');
			apikey = trim(token);
		}
		else if (token == "[URL]")
		{
			getline(pfile, token, '\n');
			serviceURL = trim(token);
		}
	}
	pfile.close();
	bot.permission.SetDefaultCommandLevel("unshorten-url", 2);
	bot.permission.SetDefaultCommandLevel("unshorten-key", 2);
}

void CmdLong::SaveState()
{
	ofstream pfile("IRCBot.unshorten");
	pfile << "[ApiKey] " << apikey << endl << "[URL] " << serviceURL << endl;
	pfile.close();
}

void CmdLong::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	if (command == "unshorten-url")
	{
		if (args == "")
		{
			bot.Say(respondto, "Unshortener service URL is currently \"" + serviceURL + "\".");
		}
		else
		{
			serviceURL = args;
			bot.Say(respondto, "Unshortener service URL set to \"" + serviceURL + "\".");
		}
		return;
	}
	else if (command == "unshorten-key")
	{
		if (args == "")
		{
			bot.Say(respondto, "You are not allowed to view the API key online, please consult the configuration.");
		}
		else
		{
			apikey = args;
			bot.Say(respondto, "Unshortener service API key to \"" + apikey + "\".");
		}
		return;
	}
	new URLLengthenerThread(bot, speaker, target, args, *this);
}

string CmdLong::HelpMsg(string command)
{
	return "Usage: long <short url> -- Returns the target of a shortened URL";
}

vector<string> CmdLong::CommandStrings()
{
	vector<string> types;
	types.push_back("long");
	types.push_back("unshorten");
	types.push_back("unshorten-url");
	types.push_back("unshorten-key");
	return types;
}

string CmdLong::GetURL(string shortURL)
{
	return stringreplace(stringreplace(serviceURL, "%u", shortURL), "%k", apikey);
}

// string URLLengthenerThread::APIkey = "ba9f50a9c58a09709f9b5c32ebf7ab5d"; // current API key.  DO NOT LOSE THIS.

URLLengthenerThread::URLLengthenerThread(IRCBot& b, Hostname& s, string t, string a, CmdLong& control) : bot(b), speaker(s), target(t), arg(a), url(control.GetURL(a))
{
	pthread_create(&thread, NULL, (void *(*)(void *)) &URLLengthenerThread::Start, this);
}

void * URLLengthenerThread::Start(void * arg)
{
	URLLengthenerThread& x =  *((URLLengthenerThread *) arg);
	x.Run();
	delete &x;
}

void URLLengthenerThread::Run()
{
	HTTPRequest r(url);
	int res = r.Do();
	string respondto = bot.GetRespondTo(speaker, target);
	if (res == -1)
		bot.Say(respondto, "An error occurred.  (File Control Error)");
	else if (res == -2)
		bot.Say(respondto, "An error occurred.  (Timed Out: Connect)");
	else if (res == -3)
		bot.Say(respondto, "An error occurred.  (Unknown Error)");
	else if (res == -4) // no longer used
		bot.Say(respondto, "An error occurred.  (No Data)");
	else if (res == -5)
		bot.Say(respondto, "An error occurred.  (Couldn't Resolve Host)");
	else if (res >= 200 && res < 300)
	{
		string link = r.GetResponse();
		if (link.find("ERROR") == 0)
			bot.Say(respondto, "An error occurred.  Perhaps that isn't a short URL?");
		else
			bot.Say(respondto, arg + " --> " + link);
	}
	else if (res > 0)
	{
		bot.Say(respondto, "Invalid HTTP response: " + tostr(res));
	}
}
