#ifndef _CMDMODULE_H_
#define _CMDMODULE_H_

#include <string>
#include <vector>
#include <set>

#include "cmd.h"

using namespace std;

class alias;
class module;

class CmdModule : public cmd
{
	public:
	CmdModule();
	void ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args);
	string HelpMsg(string command);

	void PostInstall(IRCBot& bot);
	void SaveState();

	void LoadModule(IRCBot& bot, module * mod, string modulename, Hostname speaker, string respondto);
	void UnloadModule(IRCBot& bot, module * mod, string modulename, Hostname speaker, string respondto);

	set<string> loaded;

	private:

	vector<string> CommandStrings();
};

#endif
