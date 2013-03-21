#include <sstream>
#include <fstream>

#include "CmdAlias.h"
#include "alias.h"
#include "IRCBot.h"
#include "misc.h"

CmdAlias::CmdAlias()
{
	defaultuserlevel = 5;
}

void CmdAlias::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream a(args);
	string subcommand, name;
	a >> subcommand;
	a >> name;
	subcommand = tolower(subcommand);
	name = tolower(name);
	if (subcommand == "new")
		New(bot, name, getline(a), respondto);
	else if (subcommand == "delete")
		Delete(bot, name, getline(a), respondto);
	else if (subcommand == "add")
		Add(bot, name, getline(a), respondto);
	else if (subcommand == "clear")
		Clear(bot, name, getline(a), respondto);
	else if (subcommand == "sethelp")
		SetHelp(bot, name, getline(a), respondto);
	else if (subcommand == "check")
		Check(bot, name, getline(a), respondto);
	else if (subcommand == "channel")
		SetChannelArgument(bot, name, getline(a), respondto);
	else
		bot.Say(respondto, "Invalid subcommand.  Use help alias for more info.");
}

string CmdAlias::HelpMsg(string command)
{
	return "Usage: alias <subcommand> <name> [argument] -- subcommand can be any of the following: new, delete, add, clear, sethelp, check.  Argument is only considered for sethelp (where it is the new help message) or add (where it is a command to add to the alias).  Aliases are used the same way regular commands are.";
}

vector<string> CmdAlias::CommandStrings()
{
	vector<string> types;
	types.push_back("alias");
	return types;
}

void CmdAlias::New(IRCBot& bot, string name, string args, string respondto)
{
	map<string, cmd *>::iterator i = bot.commands.find(name);
	if (i != bot.commands.end())
	{
		if (i->second->IsAlias())
			bot.Say(respondto, "An alias already exists by that name.  If you wish to create a new alias, specify a different name or delete the existing alias.");
		else
			bot.Say(respondto, "A command already exists by that name.  You must specify a different name for your alias.");
		return;
	}
	alias * al = new alias(name);
	al->Register(bot);
	aliases.insert(al);
	bot.Say(respondto, "Blank alias " + name + " created.");
	return;
}

void CmdAlias::Delete(IRCBot& bot, string name, string args, string respondto)
{
	map<string, cmd *>::iterator i = bot.commands.find(name);
	if (i == bot.commands.end())
	{
		bot.Say(respondto, "No alias exists by that name.");
		return;
	}
	else if (!i->second->IsAlias())
	{
		bot.Say(respondto, "A command exists by that name, but it is not an alias.");
		return;
	}
	alias * al = (alias *) i->second;
	bot.RemoveCommand(al->command);
	aliases.erase(al);
	delete al;
	bot.Say(respondto, "Alias " + name + " has been removed.");
	return;
}

void CmdAlias::Add(IRCBot& bot, string name, string args, string respondto)
{
	map<string, cmd *>::iterator i = bot.commands.find(name);
	if (i == bot.commands.end())
	{
		bot.Say(respondto, "No alias exists by that name.");
		return;
	}
	else if (!i->second->IsAlias())
	{
		bot.Say(respondto, "A command exists by that name, but it is not an alias.");
		return;
	}
	else if (name == tolower(args.substr(0, name.length())))
	{
		bot.Say(respondto, "You may not define recursive aliases.");
		return;
	}
	alias * al = (alias *) i->second;
	al->AddCommand(args);
	return;
}

void CmdAlias::Clear(IRCBot& bot, string name, string args, string respondto)
{
	map<string, cmd *>::iterator i = bot.commands.find(name);
	if (i == bot.commands.end())
	{
		bot.Say(respondto, "No alias exists by that name.");
		return;
	}
	else if (!i->second->IsAlias())
	{
		bot.Say(respondto, "A command exists by that name, but it is not an alias.");
		return;
	}
	alias * al = (alias *) i->second;
	al->Clear();
	return;
}

void CmdAlias::SetHelp(IRCBot& bot, string name, string args, string respondto)
{
	map<string, cmd *>::iterator i = bot.commands.find(name);
	if (i == bot.commands.end())
	{
		bot.Say(respondto, "No alias exists by that name.");
		return;
	}
	else if (!i->second->IsAlias())
	{
		bot.Say(respondto, "A command exists by that name, but it is not an alias.");
		return;
	}
	alias * al = (alias *) i->second;
	al->SetHelpMsg(args);
	return;
}

void CmdAlias::Check(IRCBot& bot, string name, string args, string respondto)
{
	map<string, cmd *>::iterator i = bot.commands.find(name);
	if (i == bot.commands.end())
		bot.Say(respondto, "No alias exists by that name.");
	else if (!i->second->IsAlias())
		bot.Say(respondto, "That is a command.");
	else
		bot.Say(respondto, "That is an alias.");
	return;
}

void CmdAlias::SetChannelArgument(IRCBot& bot, string name, string args, string respondto)
{
	map<string, cmd *>::iterator i = bot.commands.find(name);
	if (i == bot.commands.end())
	{
		bot.Say(respondto, "No alias exists by that name.");
		return;
	}
	else if (!i->second->IsAlias())
	{
		bot.Say(respondto, "A command exists by that name, but it is not an alias.");
		return;
	}
	alias * al = (alias *) i->second;
	int c = -2;
	istringstream(args) >> c;
	if (c < -1)
		bot.Say(respondto, "Invalid argument - you must specify a number.");
	else
	{
		al->SetChannelArgument(c);
		bot.Say(respondto, "Argument " + args + " will be considered when checking alias " + name + " for user flags.");
	}
	return;
}

void CmdAlias::PostInstall(IRCBot& bot)
{
	ifstream pfile("IRCBot.aliases");
	int maxsize = 512, aliascmdcount;
	char buffer[maxsize];
	string aliasname, aliashelp, aliascmd, aliaschanarg;
	while (true)
	{
		pfile >> aliasname;
		pfile >> aliaschanarg;
		pfile >> aliascmdcount;
		pfile.getline(buffer, maxsize);
		aliashelp = trim(buffer);
		if (!pfile)
			break;
		New(bot, aliasname, "", "");
		SetHelp(bot, aliasname, aliashelp, "");
		SetChannelArgument(bot, aliasname, aliaschanarg, "");
		for (int i = 0; i < aliascmdcount; i++)
		{
			pfile.getline(buffer, maxsize);
			aliascmd = buffer;
			Add(bot, aliasname, aliascmd, "");
		}
	}
	pfile.close();
}

void CmdAlias::SaveState()
{
	ofstream pfile("IRCBot.aliases");
	for (set<alias *>::iterator a = aliases.begin(); a != aliases.end(); a++)
	{
		pfile << (**a).command << " " << (**a).channelarg << " " << (**a).commands.size() << " " << (**a).helpmsg << endl;
		for (int i = 0; i < (**a).commands.size(); i++)
			pfile << (**a).commands.at(i) << endl;
	}
	pfile.close();
}
