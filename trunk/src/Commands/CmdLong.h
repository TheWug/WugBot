#ifndef _CMDLONG_H_
#define _CMDLONG_H_

#include <string>
#include <vector>
#include <pthread.h>

#include "cmd.h"

using namespace std;

class CmdLong : public cmd
{
	public:
	CmdLong();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	void PostInstall(IRCBot& b);
	void SaveState();

	string GetURL(string shortURL);

	string apikey;
	string serviceURL;

	private:
	vector<string> CommandStrings();
};

class URLLengthenerThread
{
	public:
	URLLengthenerThread(IRCBot& b, Hostname& s, string t, string a, CmdLong& control);
	static void * Start(void * arg);
	void Run();

	private:
	IRCBot& bot;

	string arg;
	string url;
	string target;
	Hostname speaker;

	pthread_t thread;
};

#endif
