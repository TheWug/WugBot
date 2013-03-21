#include <dlfcn.h>

#include "module.h"
#include "IRCBot.h"

map<string, module::ModData> module::allmodules;

module::module(IRCBot& b) : bot(b)
{
	deaf = false;
	mute = false;
}

module::~module()
{}

string module::GetName()
{
	return name;
}

void module::LoadState()
{
}

void module::SaveState()
{
}

void module::Control(string controlstring, string args, Hostname& speaker, string target, string respondto)
{
	if (controlstring == "mute")
		mute = true;
	else if (controlstring == "unmute")
		mute = false;
	else if (controlstring == "deaf")
		deaf = true;
	else if (controlstring == "undeaf")
		deaf = false;
	else
		onCommand(controlstring, args, speaker, target, Messenger(bot, respondto, mute));
}

void module::Message(string command, Hostname& speaker, string target, string respondto, string args, bool ctcpflag)
{
	if (!deaf)
		onMessage(command, speaker, target, Messenger(bot, respondto, mute), args, ctcpflag);
}

void module::onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag)
{
}

void module::onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto)
{
}

module * module::CreateModule(IRCBot& bot, string name) // needs to dynamically initialize a module from disk and attempt to instantiate it
{
	module::ModData d;

	if (allmodules.find(name) != allmodules.end()) return NULL;

	d.modptr = dlopen((string("./lib/lib") + name + string(".so")).c_str(), RTLD_NOW);
	if (d.modptr == NULL)
	{
		cout << dlerror() << endl;
		return NULL;
	}

	*((void **) (&d.constructor)) = dlsym(d.modptr, "Factory");
	if (d.constructor == NULL)
	{
		cout << dlerror() << endl;
		dlclose(d.modptr);
		return NULL;
	}
	d.instance = d.constructor(bot);
	if (d.instance == NULL)
	{
		cout << name << ": failed to get instance of object, aborting." << endl;
		dlclose(d.modptr);
		return NULL;
	}

	allmodules[name] = d;

	return d.instance;
}

void module::DeleteModule(string name) // needs to unload dynamically loaded library and deallocate existing object
{
	map<string, ModData>::iterator i = allmodules.find(name);
	if (i == allmodules.end()) return;
	delete i->second.instance;
	dlclose(i->second.modptr);
	allmodules.erase(i);
}

string module::StatusMsg()
{
	return "";
}

Messenger module::GetMessenger(string target)
{
	return Messenger(bot, target, mute);
}
