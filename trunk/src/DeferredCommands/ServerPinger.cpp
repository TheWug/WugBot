#include <string>
#include <vector>

#include "misc.h"
#include "ServerPingThread.h"
#include "ServerPinger.h"
#include "IRCBot.h"

using namespace std;

ServerPinger::ServerPinger(IRCBot& bot, Hostname speaker, string target, string user) : dcmd(speaker, target, bot)
{
	port = 25565;
	if (user.find(':') != string::npos)
	{
		int i = user.rfind(':');
		string portstr = user.substr(i + 1);
		user = user.substr(0, i);
		istringstream(portstr) >> port;
	}
	nickname = tolower(user);
	bot.Who(nickname, "%hn");
	didfind = false;
}

bool ServerPinger::Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag)
{
	if (command == "354") // who listing
	{
		vector<string> data = splitcommands(message, " ", true);
		if (tolower(data[1]) != nickname)
			return false;
		else if (data.size() == 2)
		{
			(new ServerPingThread(bot, originalspeaker, originaltarget, data[0] + ":" + tostr(port % 65536)))->start();
			didfind = true;
		}
		return true;
	}
	else if (command == "315") // end of who
	{
		if (message.find(nickname) != string::npos)
		{
			bot.Say(bot.GetRespondTo(originalspeaker, originaltarget), "Unable to determine the hostname of " + nickname + ".  Nobody with that nickname seems to be online.");
			return true;
		}
		return false;
	}
}
