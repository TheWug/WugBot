#ifndef _BOTINFO_H_
#define _BOTINFO_H_

#include <string>

using namespace std;

class BotInfo
{
	string nickname;
	string altnickname;
	string password;
	string server;
	int port;
	string servicesaccount;
	string realname;
	string username;
	string ownername;
	string connectcommands;
	string commandprefix;
	string logchannel;
	float floodminint;
	float floodeventweight;
	float floodmaxweight;
	bool readonly;

	bool usedaemon;
	bool setnickname;
	bool setaltnickname;
	bool setpassword;
	bool setserver;
	bool setport;
	bool setservicesaccount;
	bool setrealname;
	bool setusername;
	bool setownername;
	bool setconnectcommands;
	bool setcommandprefix;
	bool setfloodminint;
	bool setfloodeventweight;
	bool setfloodmaxweight;
	bool setreadonly;
	bool setlogchannel;

	public:
	BotInfo();

	void SetNick(string nickname);
	void SetUseDaemon(bool b);
	void SetAltNick(string nickname);
	void SetPass(string pass);
	void SetAccount(string acct);
	void SetRealname(string realname);
	void SetUser(string user);
	void SetOwner(string owner);
	void SetConnectCommands(string cc);
	void SetServer(string server);
	void SetCommandPrefix(string prefix);
	void SetFloodMinInterval(string interval);
	void SetFloodEventWeight(string weight);
	void SetFloodMaxWeight(string weight);
	void SetReadOnly(string weight);
	void SetLogChannel(string logchannel);

	bool GetUseDaemon() throw (string);
	string GetNick() throw (string);
	string GetAltNick() throw (string);
	string GetPass() throw (string);
	string GetAccount() throw (string);
	string GetRealname() throw (string);
	string GetUser() throw (string);
	string GetOwner() throw (string);
	string GetConnectCommands() throw (string);
	string GetServer() throw (string);
	int GetPort() throw (string);
	string GetCommandPrefix() throw (string);
	float GetFloodMinInterval() throw (string);
	float GetFloodEventWeight() throw (string);
	float GetFloodMaxWeight() throw (string);
	bool GetReadOnly() throw (string);
	string GetLogChannel() throw (string);

	bool HasEnoughInfo();
};

#endif
