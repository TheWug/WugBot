#include <set>
#include <string>

#include "Metrics.h"
#include "IRCBot.h"
#include "UserManager.h"
#include "misc.h"

extern "C" module * Factory(IRCBot& bot)
{
	return new Metrics(bot);
}

Metrics::Metrics(IRCBot& b) : module(b)
{
}

Metrics::~Metrics()
{
}

void Metrics::onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag)
{
}

void Metrics::onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto)
{
	if (controlstring == "finduser")
		FindUser(args, speaker, target, respondto);
	else if (controlstring == "help")
	{
		respondto.Say(	"Metrics Module Commands (access with \"module metrics <command> [args]\")):\n"
				"  help\n"
				"    prints this monolithic block of text\n"
				"  finduser <filters>\n"
				"    generates a list of all visible users matching the provided filters\n"
				"    Available filters:\n"
				"      -c=#chan    filters only users in the specified channel\n"
				"      -h=ho.st    filters only users matching the specified host\n"
				"      -a[=acct]   filters only users logged in as the specified account\n"
				"                  use just -a for all logged in users\n"
				"      -x          inverts the next filter\n"
				"      -[others]   not yet implemented.  other filters will be available later.");
	}
}

void Metrics::FindUser(string args, Hostname& speaker, string target, Messenger respondto)
{
	vector<string> arguments = splitcommands(args, " ", false);
	bool decision, invert = false;
	// decision == true -> keep, decision == false -> discard
	set<string>::iterator start, end;

	set<string> allvisibleusers;

	map<string, set<string> >::iterator i, e;
	for (i = bot.usermgr.occupants.begin(), e = bot.usermgr.occupants.end(); i != e; ++i)
	{
		allvisibleusers.insert(i->second.begin(), i->second.end());
	}

	for (int i = 0; i < arguments.size(); ++i)
	{
		vector<string> options = splitcommands(arguments[i], "=", false);
		if (options.size() == 0) continue;

		if (options[0] == "-x")
		{
			invert = true;
			continue;
		}
		else if (options[0] == "-c")
		{
			if (options.size() != 2)
			{
				respondto.Say("Error: badly formatted -c statement.");
				return;
			}
			e = bot.usermgr.occupants.find(options[1]);
			for (start = allvisibleusers.begin(), end = allvisibleusers.end(); start != end; ++start)
			{
				if (e == bot.usermgr.occupants.end())
				{
					decision = false;
				}
				else
					decision = e->second.find(*start) != e->second.end();

				if (!((!invert && decision) || (invert && !decision)))
					allvisibleusers.erase(start);
			}
					
		}
		else if (options[0] == "-h")
		{
			if (options.size() != 2)
			{
				respondto.Say("Error: badly formatted -h statement.");
				return;
			}
			options[1] = tolower(options[1]);
			for (start = allvisibleusers.begin(), end = allvisibleusers.end(); start != end; ++start)
			{
				decision = (tolower(bot.usermgr.GetHostnameFromNick(*start)).find(options[1]) != string::npos);

				if (!((!invert && decision) || (invert && !decision)))
					allvisibleusers.erase(start);
			}
		}
		else if (options[0] == "-a")
		{
		}
		else
		{
			respondto.Say("Unrecognized option: " + options[0]);
			return;
		}

		invert = false;
	}

	respondto.Say("Result: " + ToString(allvisibleusers));
}
