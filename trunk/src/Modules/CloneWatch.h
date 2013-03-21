#ifndef _CLONEWATCH_H_
#define _CLONEWATCH_H_

#include <string>
#include <set>
#include <map>

#include <Scheduler.h>
#include "module.h"

class IRCBot;
class Hostname;

class WatchSettings
{
	public:
	WatchSettings();

	int notifytype;
	string notify;
};

class NickEntry
{
	public:
	unsigned long long timestamp;

	string lastnick;
	string host;
	set<string> livenicks;

	bool operator==(const NickEntry& other);
	static NickEntry Dummy();

	bool OnChannel(Hostname& speaker);
	void SetOnChannel(Hostname& speaker, bool onchannel);
};

class WatchedUserDB
{
	public:
	WatchedUserDB();
	NickEntry Lookup(Hostname& speaker);
	void AddUser(Hostname& speaker);
	bool HasUser(Hostname& u);
	void UpdateNick(Hostname& speaker, string newnick, bool updatets);

	bool OnChannel(Hostname& speaker);
	void SetOnChannel(Hostname& speaker, bool onchannel);

	void Clean(int cutoff);

	private:
	map<string, NickEntry> entries;
};

class CloneWatch : public module
{
	public:
	CloneWatch(IRCBot& bot);
	~CloneWatch();

	virtual void onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag);
	virtual void onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto);

	virtual void LoadState();
	virtual void SaveState();

	WatchSettings& Settings(string s);

	NickEntry CheckDB(Hostname& speaker, string channel);

	void UpdateDB(Hostname& speaker, string newnick, bool updatets);

	void AddToAllDB(Hostname& speaker, bool onchannel);
	void AddToDB(Hostname& speaker, WatchedUserDB& chandb, bool onchannel);
	void AddToDB(Hostname& speaker, string channel, bool onchannel);
	void TestAddToDB(Hostname& speaker, string channel, bool onchannel);

	void CleanDB();

	void RegisterAlt(string nick1, string nick2);
	void UnregisterAlt(string nick1);
	bool KnownAlt(string nick1, string nick2);

	private:
	map<string, WatchSettings> settings;
	map<string, WatchedUserDB> database;
	set<string> allowed;
	event_t eventID;

	map<string, set<string> *> alts;

	pthread_mutex_t lock;

	static void * DBManagerStub(void * param);
	void DBManager();

	void Dump(NickEntry n);
};

extern "C" module * Factory(IRCBot& bot);

#endif
