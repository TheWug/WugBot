#include <map>
#include <set>
#include <string>
#include <pthread.h>
#include <deque>

#include "module.h"
#include "FloodEvents.h"
#include "Scheduler.h"

class FloodEngine;

class ChannelSettings
{
public:
	ChannelSettings();

	string reaction[10]; // the commands to call: two per infraction type
	string cleanup[10];  // the commands to call after timeout has expired, one per reaction
	float timeout[10];     // timeouts for each command to call, one per reaction

	float time_threshold[5]; // time thresholds, one per primary infraction type
	int msg_threshold[5]; // message thresholds, "

	set<string> immune; // list of users excluded from checking
};

class FloodProtectModule : public module
{
public:
	static const int N_TYPES = 5;
	static const int N_ACTIONS = 10;
	static const int MAX_CHAN_EVENTS = 20;

	FloodProtectModule(IRCBot& bot);
	~FloodProtectModule();

	void LoadState();
	void SaveState();

	virtual void onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag);
	virtual void onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto);

	string GetCommandString(string cmdstring, Hostname& speaker, string target, string reason);
	void PerformCommand(string cmdstring, Hostname& speaker, string target, string reason);
	void PerformCommandFuture(float timeout, string cmdstring, Hostname& speaker, string target, string reason);

	void Perform(Hostname& user, string target);
	void PerformToAll(Hostname& user);

	map<string, ChannelSettings> settings;

	map<string, UserInfoCollector> userevents;
	map<string, deque<ActionEvent> > actioneventsbychannel;

	static map<string, FloodType> floodtypenames;
	static map<string, FloodType> GetFloodTypeNames();

	set<string> channels;

	void AddChannelEvent(string name, ActionEvent event);

	private:
	static void * Prune(void * self); // synchronous DB scraper
	void DoPrune(); // synchronous DB scraper

	void DBLock();
	void DBUnlock();

	FloodEngine * nickhandler;
	FloodEngine * cyclehandler;
	FloodEngine * msghandler;
	FloodEngine * pinghandler;
	FloodEngine * chanhandler;

	pthread_mutex_t dblock;

	event_t pruneevent;
};

extern "C" module * Factory(IRCBot& bot);
