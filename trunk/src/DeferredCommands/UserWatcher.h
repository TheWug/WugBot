#ifndef _USERWATCHER_H_
#define _USERWATCHER_H_

#include <map>

#include "dcmd.h"

class UserWatcher : public dcmd
{
	map<string, map<string, map<string, char> > > watchlist;

	public:
	UserWatcher(IRCBot& bot);

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
	void Watch(string target, string user, string location);
};

#endif
