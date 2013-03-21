#include "ModuleHandler.h"

#include "IRCBot.h"
#include "module.h"

ModuleHandler::ModuleHandler(IRCBot& b) : bot(b)
{
}

void ModuleHandler::MarkBeginUpdate()
{
	updating = true;
	updated = set<string>();
}

void ModuleHandler::MarkEndUpdate()
{
	updating = false;
}

void ModuleHandler::UpdateModules(Hostname& s, string& c, string t, string m, bool f)
{
	MarkBeginUpdate();
	speaker = s;
	command = c;
	target = t;
	message = m;
	ctcpflag = f;

	for (map<string, module *>::iterator i = modules.begin(); i != modules.end(); ++i)
	{
		DirectUpdateModuleByName(i->first, i->second);
	}
	MarkEndUpdate();
}

void ModuleHandler::UpdateModuleByName(string modname)
{
	if (!updating) return;
	map<string, module *>::iterator i = modules.find(modname);
	if (i == modules.end()) return;
	DirectUpdateModuleByName(i->first, i->second);
}

void ModuleHandler::DirectUpdateModuleByName(string modname, module * m)
{
	if (updated.find(modname) != updated.end()) return;
	updated.insert(modname);
	m->Message(command, speaker, target, bot.GetRespondTo(speaker, target), message, ctcpflag);
}

void ModuleHandler::InstallModule(string modname)
{
}

void ModuleHandler::UninstallModule(string modname)
{
}

void ModuleHandler::CleanupModules()
{
	if (!bot.readonly)
	{
		for (map<string, module *>::iterator i = modules.begin(); i != modules.end(); i++)
			(*i).second->SaveState();
	}
	for (map<string, module *>::iterator i = modules.begin(); i != modules.end(); i++)
		delete (*i).second;
}

