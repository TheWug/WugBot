#ifndef _CMDMONITOR_H_
#define _CMDMONITOR_H_

#include <string>
#include <vector>
#include <set>
#include <map>

#include "cmd.h"

class IRCBot;
class MonitorWatcher;

using namespace std;

class CmdMonitor : public cmd
{
	public:
	CmdMonitor(IRCBot& bot);
	~CmdMonitor();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	void AddMonitorEntry(IRCBot& b, Hostname speaker, string respondto, string mtarget, string watcher);
	void RemoveMonitorEntry(IRCBot& b, Hostname speaker, string respondto, string mtarget, string watcher);

	void PostInstall(IRCBot& bot);
	void SaveState();

	private:
	map<string, set<string> > monitored;

	vector<string> CommandStrings();

	MonitorWatcher * monitorwatcher;
};

#endif
