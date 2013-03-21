#ifndef _MONITORWATCHER_H_
#define _MONITORWATCHER_H_

#include <map>
#include <set>
#include <string>

#include "dcmd.h"

using namespace std;

class MonitorWatcher : public dcmd
{
	public:
	MonitorWatcher(IRCBot& bot, map<string, set<string> >& m);
	~MonitorWatcher();

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);

	private:
	map<string, set<string> >& monitored;
};

#endif
