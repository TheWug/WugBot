#ifndef _USERMANAGER_H_
#define _USERMANAGER_H_

#include <map>
#include <set>
#include <string>

using namespace std;

class UserManager
{
	public:
	map<string, string> usernames; // for lookups of username referenced by nick
	map<string, set<string> > nicksbyuser; // for lookups of nick referenced by username

	map<string, string> hostnames; // for lookups of hostname referenced by nick
	map<string, set<string> > nicksbyhost; // for lookups of nick referenced by hostname

	map<string, string> idents; // for lookups of ident referenced by nick
	map<string, set<string> > nicksbyident; // for lookups of nick referenced by ident

	map<string, set<string> > channels;  // for lookups of channel by occupant
	map<string, set<string> > occupants; // for lookups of occupant by channel
	map<string, map<string, string> > userflags; // the flags of users in channels

	string GetUsernameFromNick(string nickname);
	set<string> GetNicknameFromUser(string nickname);
	string GetHostnameFromNick(string nickname);
	set<string> GetNicknameFromHost(string nickname);
	string GetIdentFromNick(string nickname);
	set<string> GetNicknameFromIdent(string nickname);

	Hostname GetHostname(string nick);

	void AddUser(string nickname, string ident, string host,   string username);
	void SetUser(string nickname, string newusername);
	void SetNick(string oldnickname, string newnickname);
	void RemoveUser(string username);
	void RemoveNick(string nickname);

	void UserJoinedChannel(string channel, string nickname);
	void UserLeftChannel(string channel, string nickname);
	void UserLeftAllChannels(string channel, string nickname);
	void SelfLeftChannel(string c);
	void SetChannelUFlag(string channel, string user, bool setflag, char flag);
	void ClearChannelUFlags(string channel, string user);

	bool UserHasFlag(string channel, string user, char flag);
	string UserFlagString(string channel, string user);

	void ReadState();
	void SaveState();
};

#endif
