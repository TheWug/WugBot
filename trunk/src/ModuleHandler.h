#ifndef _MODULEHANDLER_H_
#define _MODULEHANDLER_H_

#include <string>
#include <set>
#include <map>

#include "Hostname.h"

using namespace std;

class IRCBot;
class module;

class ModuleHandler
{
	public:
	ModuleHandler(IRCBot&);

	void MarkBeginUpdate();
	void MarkEndUpdate();
	void UpdateModules(Hostname& s, string& c, string t, string m, bool f);
	void UpdateModuleByName(string modname);

	void InstallModule(string modname);
	void UninstallModule(string modname);

	void CleanupModules();

	set<string> updated;
	map<string, module *> modules;

	bool updating;

	IRCBot& bot;

	Hostname speaker;
	string command;
	string target;
	string message;
	bool ctcpflag;

	private:
	void DirectUpdateModuleByName(string modname, module * m);
};

#endif
