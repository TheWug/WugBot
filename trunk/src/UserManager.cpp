#include <iostream>
#include <fstream>

#include "Hostname.h"
#include "UserManager.h"
#include "misc.h"

string UserManager::GetUsernameFromNick(string nickname)
{
	map<string, string>::iterator username = usernames.find(nickname);
	if (username != usernames.end())
		return (*username).second;
	return "";
}

set<string> UserManager::GetNicknameFromUser(string username)
{
	map<string, set<string> >::iterator nickname = nicksbyuser.find(username);
	if (nickname != nicksbyuser.end())
		return (*nickname).second;
	return set<string>();
}

string UserManager::GetHostnameFromNick(string nickname)
{
	map<string, string>::iterator hostname = hostnames.find(nickname);
	if (hostname != hostnames.end())
		return (*hostname).second;
	return "";
}

set<string> UserManager::GetNicknameFromHost(string hostname)
{
	map<string, set<string> >::iterator nickname = nicksbyhost.find(hostname);
	if (nickname != nicksbyhost.end())
		return (*nickname).second;
	return set<string>();
}

string UserManager::GetIdentFromNick(string nickname)
{
	map<string, string>::iterator ident = idents.find(nickname);
	if (ident != idents.end())
		return (*ident).second;
	return "";
}

set<string> UserManager::GetNicknameFromIdent(string ident)
{
	map<string, set<string> >::iterator nickname = nicksbyident.find(ident);
	if (nickname != nicksbyident.end())
		return (*nickname).second;
	return set<string>();
}

Hostname UserManager::GetHostname(string nick)
{
	return Hostname(nick, GetIdentFromNick(nick), GetHostnameFromNick(nick), GetUsernameFromNick(nick));
}

void UserManager::AddUser(string nick, string ident, string host, string user)
{
//	cout << "ADDUSER: " << nick << " " << ident << " " << host << " " << user << endl;
	if (user == "0")
		user = Hostname::unauthenticated;
	if (usernames.find(nick) != usernames.end())
		return;
	usernames[nick] = user;
	nicksbyuser[user].insert(nick);
	hostnames[nick] = host;
	nicksbyhost[host].insert(nick);
	idents[nick] = ident;
	nicksbyident[ident].insert(nick);
}

void UserManager::SetUser(string nickname, string newusername)
{
//	cout << "SETUSER: " << nickname << " " << newusername << endl;
	if (newusername == "0")
		newusername = "*";
	usernames.erase(nickname);
	usernames[nickname] = newusername;
}

void UserManager::SetNick(string oldnickname, string newnickname)
{
//	cout << "SETNICK: " << oldnickname << " " << newnickname << endl;
	map<string, string>::iterator user = usernames.find(oldnickname);
	map<string, string>::iterator host = hostnames.find(oldnickname);
	map<string, string>::iterator id = idents.find(oldnickname);

	string username = user->second;
	string hostname = host->second;
	string ident = id->second;
	nicksbyuser[username].erase(oldnickname);
	nicksbyuser[username].insert(newnickname);
	nicksbyhost[hostname].erase(oldnickname);
	nicksbyhost[hostname].insert(newnickname);
	nicksbyident[ident].erase(oldnickname);
	nicksbyident[ident].insert(newnickname);
	string temp = usernames[oldnickname];
	usernames.erase(oldnickname);
	usernames[newnickname] = temp;
	temp = hostnames[oldnickname];
	hostnames.erase(oldnickname);
	hostnames[newnickname] = temp;
	temp = idents[oldnickname];
	idents.erase(oldnickname);
	idents[newnickname] = temp;
	channels[newnickname] = channels[oldnickname];
	channels.erase(oldnickname);
	for (set<string>::iterator i = channels[newnickname].begin(); i != channels[newnickname].end(); i++)
	{
		string c = *i;
		int i = occupants[c].erase(oldnickname);
		if (i != 1)
			cout << "error: could not find specified user in channel" << endl;
		occupants[c].insert(newnickname);
	}
}

void UserManager::RemoveNick(string oldnick)
{
//	cout << "REMOVENICK: " << oldnick << endl;
	string olduser = GetUsernameFromNick(oldnick);
	string oldhost = GetHostnameFromNick(oldnick);
	string oldident = GetIdentFromNick(oldnick);

	map<string, set<string> >::iterator unick = nicksbyuser.find(olduser);
	map<string, string>::iterator user = usernames.find(oldnick);
	map<string, set<string> >::iterator hnick = nicksbyhost.find(olduser);
	map<string, string>::iterator host = hostnames.find(oldhost);
	map<string, set<string> >::iterator inick = nicksbyident.find(olduser);
	map<string, string>::iterator ident = idents.find(oldident);

	if (unick != nicksbyuser.end())
	{
		nicksbyuser[olduser].erase(oldnick);
		if (nicksbyuser[olduser].size() == 0)
			nicksbyuser.erase(olduser);
	}
	if (user != usernames.end())
		usernames.erase(oldnick);

	if (unick != nicksbyhost.end())
	{
		nicksbyhost[oldhost].erase(oldnick);
		if (nicksbyhost[oldhost].size() == 0)
			nicksbyhost.erase(olduser);
	}
	if (host != hostnames.end())
		hostnames.erase(oldnick);

	if (inick != nicksbyident.end())
	{
		nicksbyident[oldident].erase(oldnick);
		if (nicksbyident[oldident].size() == 0)
			nicksbyident.erase(olduser);
	}
	if (ident != idents.end())
		idents.erase(oldnick);
}

void UserManager::UserJoinedChannel(string c, string u)
{
	//cout << "USERJOINEDCHANNEL: " << c << " " << u << endl;
	channels[u].insert(c);
	occupants[c].insert(u);
}

void UserManager::UserLeftChannel(string c, string u)
{
	//cout << "USERLEFTCHANNEL: " << c << " " << u << endl;
	channels[u].erase(c);
	occupants[c].erase(u);
	if (channels[u].size() == 0)
	{
		channels.erase(u);
		RemoveNick(u);
	}
	if (occupants[c].size() == 0)
		occupants.erase(c);
}

void UserManager::SelfLeftChannel(string c)
{
	//cout << "SELFLEFTCHANNEL: " << c << endl;
	set<string> occupantscopy;
	occupantscopy.insert(occupants[c].begin(), occupants[c].end());
	for (set<string>::iterator occ = occupantscopy.begin(); occ != occupantscopy.end(); occ++)
		UserLeftChannel(c, *occ);
}

void UserManager::UserLeftAllChannels(string c, string u)
{
	//cout << "USERLEFTALLCHANNELS: " << c << " " << u << endl;
	set<string> channelscopy;
	channelscopy.insert(channels[u].begin(), channels[u].end());
	for (set<string>::iterator chan = channelscopy.begin(); chan != channelscopy.end(); chan++)
		UserLeftChannel(*chan, u);
}

void UserManager::SetChannelUFlag(string channel, string user, bool setflag, char flag)
{
	string& flags = (userflags[channel])[user];
	char flagstr[2];
	flagstr[0] = flag;
	flagstr[1] = 0;
	if (setflag)
	{
		if (flags.find(flag) == flags.npos)
			flags.append(flagstr);
	}
	else
	{
		flags = stringreplace(flags, flagstr, "");
		if (flags == "")
			ClearChannelUFlags(channel, user);
	}
}

void UserManager::ClearChannelUFlags(string channel, string user)
{
	userflags[channel].erase(user);
	if (userflags[channel].size() == 0)
		userflags.erase(channel);
}

bool UserManager::UserHasFlag(string channel, string user, char flag)
{
	string& flags = (userflags[channel])[user];
	if (flag != 'o' && flag != 'v')
		return (flags.find(flag) != flags.npos);
	else
		return ((flags.find(flag) != flags.npos) && (flags.find('x') == flags.npos));
}

string UserManager::UserFlagString(string channel, string user)
{
	return (userflags[channel])[user];
}

void UserManager::ReadState()
{
	ifstream pfile("IRCBot.flags");
	string name, chan, flags;
	while (true)
	{
		pfile >> chan;
		pfile >> name;
		pfile >> flags;
		if (!pfile)
			break;
		userflags[chan][name] = flags;
	}
	pfile.close();
}

void UserManager::SaveState()
{
	ofstream pfile("IRCBot.flags");
	for (map<string, map<string, string> >::iterator i = userflags.begin(); i != userflags.end(); i++)
	{
		for (map<string, string>::iterator j = i->second.begin(); j != i->second.end(); j++)
		{
			if (j->second != "")
				pfile << i->first << " " << j->first << " " << j->second << endl;
		}
	}
	pfile.close();
}
