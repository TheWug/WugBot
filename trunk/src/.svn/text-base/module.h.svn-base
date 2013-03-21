#ifndef _MODULE_H_
#define _MODULE_H_

#include <string>
#include <vector>
#include <map>

#include "Hostname.h"
#include "Messenger.h"

using namespace std;

class IRCBot;

class module
{
public:
	module(IRCBot& bot);
	virtual ~module();

	string GetName();
	virtual void LoadState();
	virtual void SaveState();

	void Message(string command, Hostname& speaker, string target, string respondto, string args, bool ctcpflag);
	void Control(string controlstring, string args, Hostname& speaker, string target, string respondto);

	virtual string StatusMsg();

	static module * CreateModule(IRCBot& bot, string name);
	static void DeleteModule(string name);

	IRCBot& bot;

	Messenger GetMessenger(string target);

private:

	class ModData
	{
	public:
		ModData() {}

		void * modptr;
		module * (* constructor)(IRCBot& bot);
		module * instance;
	};

	static map<string, ModData> allmodules;

	bool deaf, mute;

	virtual void onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag);
	virtual void onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto);

	protected:
	string name;
};

#endif
