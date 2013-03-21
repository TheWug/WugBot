#ifndef _CMDQUEUE_H_
#define _CMDQUEUE_H_

#include <string>
#include <list>
#include <vector>
#include <map>

#include "cmd.h"

using namespace std;

class CmdQueue : public cmd
{
	public:

	CmdQueue();
	typedef void (CmdQueue::*queuefunction)(IRCBot&, string, string, Hostname, string, string, bool);

	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	void Add(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak = false);
	void Remove(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak = false);
	void Pop(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak = false);
	void Bump(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak = false);
	void Sink(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak = false);
	void Invite(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak = false);
	void Kick(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak = false);
	void Where(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak = false);
	void List(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak = false);

	map<string, list<string> > queues;
	map<string, queuefunction> functions;
	map<string, int> levelreqs;

	void PostInstall(IRCBot& bot);
	void SaveState();

	private:
	vector<string> CommandStrings();
};

#endif
