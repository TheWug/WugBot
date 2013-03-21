#include <iostream>
#include <sstream>
#include <fstream>

#include "CmdModule.h"
#include "module.h"
#include "IRCBot.h"
#include "ModuleHandler.h"
#include "misc.h"

CmdModule::CmdModule()
{
	defaultuserlevel = -1;
}

void CmdModule::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	string mod, modcommand, modargs;
	istringstream s(args);
	s >> mod;
	s >> modcommand;
	mod = tolower(mod);
	modcommand = tolower(modcommand);
	modargs = trim(getline(s));
	map<string, module *>::iterator m = bot.modhandler.modules.find(mod);
	module * selectedmodule = NULL;
	if (m != bot.modhandler.modules.end())
		selectedmodule = m->second;
	if (mod == "list" && modcommand == "")
	{
		string loadedmodules;
		for (set<string>::iterator i = loaded.begin(); i != loaded.end(); ++i) loadedmodules += " " + *i;
		if (loadedmodules == "") bot.Say(respondto, "No modules loaded.");
		else bot.Say(respondto, "Modules loaded:" + loadedmodules);
	}
	else if (modcommand == "" && selectedmodule != NULL)
		bot.Say(respondto, selectedmodule->StatusMsg());
	else if (modcommand == "load")
		LoadModule(bot, selectedmodule, mod, speaker, respondto);
	else if (modcommand == "unload")
		UnloadModule(bot, selectedmodule, mod, speaker, respondto);
	else if (selectedmodule != NULL)
		selectedmodule->Control(modcommand, modargs, speaker, target, respondto);
	else
		bot.Say(respondto, "Module not found.  Perhaps it isn't loaded?");
}

string CmdModule::HelpMsg(string command)
{
	return "Usage: " + command + " <module> <command> [arguments ...] -- Controls a module.  All modules support the following commands: deaf, undeaf, mute, unmute, load, unload.  Some modules may support other commands.";
}

vector<string> CmdModule::CommandStrings()
{
	vector<string> types;
	types.push_back("module");
	types.push_back("mod");
	return types;
}

void CmdModule::LoadModule(IRCBot& bot, module * mod, string modulename, Hostname speaker, string respondto)
{
	if (mod != NULL)
		bot.Say(respondto, "Module already loaded.");
	else
	{
		loaded.insert(modulename);
		mod = module::CreateModule(bot, modulename);
		if (mod == NULL)
		{
			bot.Say(respondto, "No module by that name exists.");
			return;
		}
		mod->LoadState();
		bot.Say(respondto, "Loaded and initialized module: " + modulename);
		bot.modhandler.modules[modulename] = mod;
	}
}

void CmdModule::UnloadModule(IRCBot& bot, module * mod, string modulename, Hostname speaker, string respondto)
{
	if (mod == NULL)
		bot.Say(respondto, "Module not loaded.");
	else
	{
		bot.modhandler.modules.erase(modulename);
		bot.Say(respondto, "Deinitialized and unloaded module: " + modulename);
		mod->SaveState();
		module::DeleteModule(modulename);
		loaded.erase(modulename);
	}
}

void CmdModule::PostInstall(IRCBot& bot)
{
	string mod;
	module * m;
	ifstream pfile("IRCBot.modules");
	pfile >> mod;
	while (pfile)
	{
		m = module::CreateModule(bot, mod);
		if (m == NULL)
		{
			cout << "Unable to load startup module: " << mod << endl;
			pfile >> mod;
			continue;
		}
		loaded.insert(mod);
		m->LoadState();
		bot.modhandler.modules[mod] = m;
		cout << "Startup module loaded: " << mod << endl;
		pfile >> mod;
	}
}

void CmdModule::SaveState()
{
	ofstream pfile("IRCBot.modules");
	for (set<string>::iterator i = loaded.begin(); i != loaded.end(); ++i)
		pfile << *i << endl;
	pfile.close();
}

