#include <string>

#include "WhoTracker.h"
#include "IRCBot.h"
#include "UserManager.h"
#include "misc.h"

using namespace std;

WhoTracker::WhoTracker(string chan, IRCBot& bot) : dcmd(Hostname(""), "", bot)
{
	channel = chan;
	error = false;
	bot.Who(channel, "%cnfauh");
}

bool WhoTracker::Call(string command, Hostname s, string t, string respondto, string message, bool ctcpflag)
{
	if (command == "354")
	{
		vector<string> data = splitcommands(message, " ", true);
		if (data[0] != channel)
			return false;
		if (data.size() < 4 && error == false)
		{
			cout << "Warning:  Invalid who response.  User tracking may be out of sync!" << endl;
			error = true;
		}
		else
		{
			bot.usermgr.UserJoinedChannel(data[0], data[3]);
			bot.usermgr.AddUser(data[3], data[1], data[2], data[5]);
		}
		return false;
	}
	else if (command == "315" && message.find(channel) != string::npos)
		return true;
	return false;
}

string WhoTracker::GetFlags(string s)
{
	string output;
	if (s.find('@') != -1)
		output.append("o");
	if (s.find('+') != -1)
		output.append("v");
	return output;
}
