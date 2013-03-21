#include <string>

#include "IPReader.h"
#include "IRCBot.h"
#include "misc.h"

IPReader::IPReader(string c, Hostname s, string ot, string t, IRCBot& bot) : dcmd(s, ot, bot)
{
	bot.Who(t, "%ahin");
	request = c;
	target = tolower(t);
	didfind = false;
}

bool IPReader::Call(string command, Hostname s, string t, string r, string message, bool c)
{
	if (command == "354")
	{
		vector<string> data = splitcommands(message, " ", true);
		if (data.size() < 4)
			return false;
		else if (tolower(data[2]) != target)
			return false;
		else
		{
			didfind = true;
			ip = data[0];
			host = data[1];
			target = data[2];
			account = data[3];
			if (request == "ip")
				bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "IP address of " + target + " is: " + ip);
			else if (request == "host")
				bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "Hostname of " + target + " is: " + host);
			else if (request == "account")
			{
				if (account != "0" && account != "*")
					bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), target + " is logged in as: " + account);
				else
					bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), target + " is not logged in.");
			}
		}
		return true;
	}
	else if (command == "315")
	{
		string m = tolower(message);
		if (m.find(target) != m.npos)
		{
			if (request == "ip")
				bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "Unable to determine IP address of " + target + ".  Are you sure you typed it right?");
			else if (request == "host")
				bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "Unable to determine hostname of " + target + ".  Are you sure you typed it right?");
			else if (request == "account")
				bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "Unable to determine account of " + target + ".  Are you sure you typed it right?");
			return true;
		}
	}
	return false;
}
