#ifndef _FACTOIDPROCESSOR_H_
#define _FACTOIDPROCESSOR_H_

#include <map>
#include <vector>
#include <set>

#include "dcmd.h"

class FactoidProcessor : public dcmd
{
	public:

	map<string, string> factoids;
	map<string, string> setters;
	map<string, int> locklevels;
	
	set<string> blockedchannels;
	set<string> bannedusers;

	FactoidProcessor(IRCBot& b) : dcmd(Hostname(""), "", b) {}

	int AddFactoid(string factoid, string data, Hostname caller, string target, string respondto);
	int AppendFactoid(string factoid, string newdata, Hostname caller, string target, string respondto);
	int RemoveFactoid(string factoid, Hostname caller, string target, string respondto);
	int ModifyFactoid(string factoid, string newdata, Hostname caller, string target, string respondto);

	int Enable(string location);
	int Disable(string location);

	int Ban(string user);
	int Unban(string user);

	int Lock(string factoid, int locklevel);
	int Unlock(string factoid, int unlocklevel);

	bool HasFactoid(string factoid);
	bool HasAccess(Hostname user, string factoid);

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
	string GetFactoidMessage(string factoidname, string rtype, Hostname& speaker, string channel, string specifiedtarget, bool cansendtotarget, bool cansendtochannel, bool isquery);

	void Search(vector<string>& result, string query);

	void SaveFactoidDB();
	void LoadFactoidDB();

	int linelimit;
	const static int OK = 0, BADSTATE = 1, LOCKED = 2, BANNED = 3, CHANBANNED = 4;
};

#endif
