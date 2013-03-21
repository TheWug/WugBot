#include <string>
#include <iostream>
#include <fstream>

#include "BotPermissions.h"
#include "IRCBot.h"
#include "misc.h"

using namespace std;

const string BotPermissions::B_NICK = "N:";
const string BotPermissions::B_USER = "U:";
const string BotPermissions::B_ACCT = "A:";
const string BotPermissions::B_HOST = "H:";

BotPermissions::BotPermissions()
{
	LoadFromFile();
}

void BotPermissions::LoadFromFile()
{
}

bool BotPermissions::CanBeUsedBy(Hostname& user)
{
	return (
		ignoredusers.find(B_NICK + user.GetNickL()) == ignoredusers.end()
	&&	ignoredusers.find(B_HOST + user.GetHostL()) == ignoredusers.end()
	&&	ignoredusers.find(B_ACCT + user.GetAccountL()) == ignoredusers.end()
	&&	ignoredusers.find(B_USER + user.GetUserL()) == ignoredusers.end()
		);
}

bool BotPermissions::CanSendTo(string target)
{
	return (targetmutelist.find(tolower(target)) == targetmutelist.end());
}

void BotPermissions::MuteToTarget(string target)
{
	targetmutelist.insert(tolower(target));
}

void BotPermissions::UnmuteToTarget(string target)
{
	targetmutelist.erase(tolower(target));
}

bool BotPermissions::CanBeIn(string channel)
{
	return (channelnogolist.find(tolower(channel)) == channelnogolist.end());
}

void BotPermissions::BanFromChannel(string channel)
{
	channelnogolist.insert(tolower(channel));
}

void BotPermissions::UnbanFromChannel(string channel)
{
	channelnogolist.erase(tolower(channel));
}

bool BotPermissions::SpamEnabled(string channel)
{
	return (spamdenied.find(tolower(channel)) == spamdenied.end());
}

void BotPermissions::AllowSpam(string channel)
{
	spamdenied.erase(tolower(channel));
}

void BotPermissions::DenySpam(string channel)
{
	spamdenied.insert(tolower(channel));
}

bool BotPermissions::UserBanned(Hostname user)
{
	return !CanBeUsedBy(user);
}

void BotPermissions::BanUser(Hostname user)
{
	if (user.GetNickL() != "")
		ignoredusers.insert(B_NICK + user.GetNickL());
	if (user.GetUserL() != "")
		ignoredusers.insert(B_USER + user.GetUserL());
	if (user.GetHostL() != "")
		ignoredusers.insert(B_HOST + user.GetHostL());
	if (user.GetAccountL() != "")
		ignoredusers.insert(B_ACCT + user.GetAccountL());
}

void BotPermissions::UnbanUser(Hostname user)
{
	ignoredusers.erase(B_NICK + user.GetNickL());
	ignoredusers.erase(B_USER + user.GetUserL());
	ignoredusers.erase(B_HOST + user.GetHostL());
	ignoredusers.erase(B_ACCT + user.GetAccountL());
}

bool BotPermissions::UserBanned(string btype, string userid)
{
	return (ignoredusers.find(btype + tolower(userid)) != ignoredusers.end());
}

void BotPermissions::BanUser(string btype, string target)
{
	ignoredusers.insert(btype + tolower(target));
}

void BotPermissions::UnbanUser(string btype, string target)
{
	ignoredusers.erase(btype + tolower(target));
}

void BotPermissions::SetUserLevel(string user, int level)
{
	userlevels[user] = level;
}

int BotPermissions::GetUserLevel(string user)
{
	map<string, int>::iterator item = userlevels.find(user);
	if (item != userlevels.end())
		return item->second;
	return 0;
}

void BotPermissions::SetCommandLevel(string command, int level)
{
	commandlevels[command] = level;
}

void BotPermissions::SetDefaultCommandLevel(string command, int level)
{
	defaultcommandlevels[command] = level;
}

string BotPermissions::GetOwner()
{
	return botowner;
}

void BotPermissions::SetOwner(string newowner)
{
	botowner = newowner;
}

bool BotPermissions::CanUseCommand(string user, string command)
{
	return ((GetUserLevel(user) >= GetCommandLevel(command) && GetCommandLevel(command) != -1) || user == GetOwner());
}

int BotPermissions::GetCommandLevel(string command)
{
	map<string, int>::iterator item = commandlevels.find(command);
	if (item != commandlevels.end())
		return item->second;
	else
	{
		item = defaultcommandlevels.find(command);
		if (item != defaultcommandlevels.end())
			return item->second;
	}
	return 0;
}

void BotPermissions::SetCommandFlags(string& command, string flags)
{
	string temp;
	char tempstr[2];
	tempstr[1] = 0;
	for (string::iterator i = flags.begin(); i != flags.end(); i++)
	{
		if (IsAlphaNumeric(*i))
		{
			tempstr[0] = *i;
			temp.append(tempstr);
		}
	}
	commandflagreqs[command] = temp;
}

string BotPermissions::GetCommandFlags(string command)
{
	map<string, string>::iterator item = commandflagreqs.find(command);
	if (item != commandflagreqs.end())
		return item->second;
	return "";
}

void BotPermissions::ReadState()
{
	int maxsize = 512;
	char buffer[maxsize];
	int type;
	ifstream pfile("IRCBot.permissions");
	while (pfile)
	{
		pfile.getline(buffer, maxsize);
		string line = buffer;
		if (CheckType(type, line))
			continue;
		Read(type, line);
	}
	pfile.close();
}

bool BotPermissions::CheckType(int& type, string line)
{
	if (line == "[UserLevels]")
		type = 0;
	else if (line == "[CommandLevels]")
		type = 1;
	else if (line == "[CommandFlags]")
		type = 2;
	else if (line == "[MuteList]")
		type = 3;
	else if (line == "[NoJoinList]")
		type = 4;
	else if (line == "[IgnoredList]")
		type = 5;
	else if (line == "[NoChatList]")
		type = 6;
	else
		return false;
	return true;
}

void BotPermissions::Read(int type, string line)
{
	string key, value;
	int valueint;
	istringstream i(line);
	if (type == 0)
	{
		i >> key;
		i >> valueint;
		userlevels[key] = valueint;
	}
	else if (type == 1)
	{
		i >> key;
		i >> valueint;
		commandlevels[key] = valueint;
	}
	else if (type == 2)
	{
		i >> key;
		i >> value;
		commandflagreqs[key] = value;
	}
	else if (type == 3)
	{
		while (i)
		{
			i >> value;
			targetmutelist.insert(value);
		}
	}
	else if (type == 4)
	{
		while (i)
		{
			i >> value;
			channelnogolist.insert(value);
		}
	}
	else if (type == 5)
	{
		while (i)
		{
			i >> value;
			ignoredusers.insert(value);
		}
	}
	else if (type == 6)
	{
		while (i)
		{
			i >> value;
			spamdenied.insert(value);
		}
	}
}

void BotPermissions::SaveState()
{
	ofstream pfile("IRCBot.permissions");
	pfile << "[UserLevels]" << endl;
	for (map<string, int>::iterator ul = userlevels.begin(); ul != userlevels.end(); ul++)
		pfile << ul->first << ' ' << ul->second << endl;

	pfile << "[CommandLevels]" << endl;
	for (map<string, int>::iterator cl = commandlevels.begin(); cl != commandlevels.end(); cl++)
	{
		map<string, int>::iterator f = defaultcommandlevels.find(cl->first);
		if (f != defaultcommandlevels.end() && cl->second != f->second)
			pfile << cl->first << ' ' << cl->second << endl;
	}

	pfile << "[CommandFlags]" << endl;
	for (map<string, string>::iterator cf = commandflagreqs.begin(); cf != commandflagreqs.end(); cf++)
	{
		if (cf->second != "")
			pfile << cf->first << ' ' << cf->second << endl;
	}

	pfile << "[MuteList]" << endl;
	for (set<string>::iterator ml = targetmutelist.begin(); ml != targetmutelist.end(); ml++)
		pfile << *ml << ' ';
	pfile << endl;

	pfile << "[NoJoinList]" << endl;
	for (set<string>::iterator ml = channelnogolist.begin(); ml != channelnogolist.end(); ml++)
		pfile << *ml << ' ';
	pfile << endl;

	pfile << "[IgnoredList]" << endl;
	for (set<string>::iterator ml = ignoredusers.begin(); ml != ignoredusers.end(); ml++)
		pfile << *ml << ' ';
	pfile << endl;

	pfile << "[NoChatList]" << endl;
	for (set<string>::iterator ml = spamdenied.begin(); ml != spamdenied.end(); ml++)
		pfile << *ml << ' ';
	pfile << endl;
	pfile.close();
}

