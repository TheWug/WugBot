#ifndef _BOTPERMISSIONS_H_
#define _BOTPERMISSIONS_H_

#include <string>
#include <vector>
#include <map>
#include <set>

#include "Hostname.h"

class IRCBot;

using namespace std;

class BotPermissions
{
	string botowner;

	map<string, int> userlevels;
	map<string, int> commandlevels;
	map<string, int> defaultcommandlevels;
	map<string, string> commandflagreqs;

	set<string> targetmutelist;
	set<string> channelnogolist;
	set<string> ignoredusers;
	set<string> spamdenied;


	public:

	static const string B_NICK, B_HOST, B_ACCT, B_USER;

	BotPermissions();

	void LoadFromFile();

	bool CanBeUsedBy(Hostname& user);
	bool CanUseCommand(string user, string command);

	bool CanSendTo(string channel);
	void MuteToTarget(string channel);
	void UnmuteToTarget(string channel);

	bool CanBeIn(string channel);
	void BanFromChannel(string channel);
	void UnbanFromChannel(string channel);

	bool SpamEnabled(string channel);
	void AllowSpam(string channel);
	void DenySpam(string channel);

	bool UserBanned(Hostname user);
	void BanUser(Hostname user);
	void UnbanUser(Hostname user);

	bool UserBanned(string btype, string user);
	void BanUser(string btype, string user);
	void UnbanUser(string btype, string user);

	void SetUserLevel(string user, int level);
	int GetUserLevel(string user);

	void SetCommandLevel(string command, int level);
	void SetDefaultCommandLevel(string command, int level);
	int GetCommandLevel(string command);

	void SetCommandFlags(string& command, string newflags);
	string GetCommandFlags(string command);

	string GetOwner();
	void SetOwner(string newowner);

	void ReadState();
	void SaveState();

	bool CheckType(int& type, string line);
	void Read(int type, string line);
};

#endif
