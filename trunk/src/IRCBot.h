#ifndef _IRCBOT_H_
#define _IRCBOT_H_

#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>

#include "IRCBotNetIface.h"
#include "Hostname.h"

class ServerProperties;
class BotPermissions;
class BotInfo;
class UserManager;
class cmd;
class dcmd;
class module;
class rsp;
class ModuleHandler;
class Timer;
class Scheduler;

using namespace std;

class IRCBot
{
	private:
	

	public:
	IRCBotNetIface& network;
	BotPermissions& permission;
	ServerProperties& serverproperties;
	UserManager& usermgr;
	ModuleHandler& modhandler;
	Scheduler& scheduler;

	string nick, altnick;
	string lnick;
	string nsaccount;
	string nspassword;
	string realname;
	string user;
	string servername;
	string logtarget;

	bool authenticated;
	bool connected;
	bool shuttingdown;
	bool registered;

	bool readonly;
	bool silent;

	vector<string> received;
	vector<string> sent;	
	map<string, string> ctcpresponses;

	vector<string> connectcommands;

	map<string, cmd *> commands;
	set<cmd *> registeredcommands;
	set<dcmd *> deferred;

	set<string> channellist;

	map<string, rsp *> responses;
	char commandprefix;

	IRCBot(BotInfo& b); // constructor/destructor
	~IRCBot();

	void ReadConnectCommands(string ccfilename); // config

	void NotifyPermissionUpdate(); // unused?

	void InitialConnect(string server, int port); // exposed
	void Reconnect(); // exposed
	void ProcessLoop(); // exposed
	void FinishConnect(); // exposed
	
	void Process(string response); // response handling
	void ProcessFrom(string origin, string response); // response handling
	void ProcessFromCmd(Hostname& origin, string& command, string response); // response handling
	void ProcessInputFirst(Hostname& speaker, string& command, string, string message, bool ctcpflag); // response handling
	void ProcessInputSecond(Hostname& speaker, string& command, string, string message, bool ctcpflag); // response handling
	void ProcessCTCP(Hostname& speaker, string& command, string, string& ctcpcommand, string ctcpargs); // response handling

	bool ParseCommand(Hostname& speaker, string target, string respondto, string message); // command handling
	bool PerformCommand(Hostname& speaker, string target, string respondto, string message); // command handling
	void ChatResponse(Hostname& speaker, string target, string respondto, string& message); // command handling
	bool ProcessDeferred(Hostname speaker, string command, string target, string message, bool ctcpflag); // command handling
	void ProcessModule(Hostname& speaker, string& command, string, string message, bool ctcpflag); // command handling

	void Say(string target, string message); // io
	void Message(string target, string message); // io
	void Notice(string target, string message); // io
	void Action(string target, string message); // io
	void Raw(string data); // io
	void NSAuth(); // controlled io
	void CTCPReply(string speaker, string& message); // io
	void Join(string channel); // io
	void Nick(string newnick); // io
	void Mode(string target, string mode); // io
	void Who(string channel, string arguments); // io
	void Kick(string nick, string channel, string reason); // io
	void Whois(string nick); // io
	void Part(string channel, string message); // io
	void Quit(string message); // io
	void Pong(string request); // io
	void ProcessNamesList(string message); // io support
	void WhoChannel(string channel); // io support
	void Topic(string channel, string newtopic); // io
	void Monitor(string action, string target); // io
	void Register(); // io support
	void Log(string data); // io (should be renamed)

	string GetCTCPResponse(string ctcp, Hostname user, string args); // internal

	void OnChannelEnter(string channel); // event
	void OnChannelLeave(string channel); // event
	void OnNickChange(string newnick); // event

	void AddCommand(string id, cmd * newcommand, int userlevel, string userflags); // command handling
	void RemoveCommand(string id); // command handling
	void CleanupCommands(); // shutdown
	void CleanupDeferredCommands(); // shutdown (broken)
	void CleanupModules(); // shutdown

	bool DoCommand(string& command, Hostname& speaker, string target, string respondto, string input, bool takesflags = false); // command handling
	bool TakesFlags(string command); //

	string GetRespondTo(Hostname speaker, string target); // io support

	void AddDeferredCommand(dcmd * d); // command handling
	void RemoveDeferredCommand(dcmd * d); // command handling

	void SetResponse(string respondto, rsp * response); // io handling
};

#endif
