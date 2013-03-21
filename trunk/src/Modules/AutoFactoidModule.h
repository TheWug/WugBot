#ifndef _AUTOFACTOIDMODULE_H_
#define _AUTOFACTOIDMODULE_H_

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>

#include "module.h"
#include "Stat.h"

class AutoFactoidModule : public module
{
	public:
	AutoFactoidModule(IRCBot& bot);

	void LoadState();
	void SaveState();

	vector<string> WasFactoidCalled(string data); // determines if a line calls a factoid.  if true, vector contains [factoid, target], else, vector is empty.
	string MatchLineWithDB(string line); // matches a line against all entries in the database in an attempt to find a matching factoid.
	void InsertIntoDatabase(string factoid, string newdata, bool force = false); // inserts a string into the database under the specified factoid.
	void RemoveFromDatabase(string factoid, string data);

	void onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag);
	void onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto);

	void CallFactoid(string factoid, Hostname speaker, Messenger respondto);
	float GetSimilarityIndex(string factoid, string line);
	bool IsEmpty(string factoid);

	static module * Factory(IRCBot& bot);

	set<string> channelwhitelist;
	set<string> factoidblacklist;
	set<string> userblacklist;

	map<string, FuzzyMatchDB> afdatabase;

	static const int MAXSIZE = 3;
	static const int ID;

	bool islogging;
};

#endif
