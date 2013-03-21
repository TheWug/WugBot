#include <fstream>
#include <cmath>

#include "AutoFactoidModule.h"
#include "Stat.h"
#include "LineDB.h"
#include "misc.h"
#include "IRCBot.h"
#include "ModuleHandler.h"

#include "CmdFactoid.h"
#include "FactoidProcessor.h"

const int AutoFactoidModule::ID = module::GetID("autofactoid", AutoFactoidModule::Factory);

AutoFactoidModule::AutoFactoidModule(IRCBot& bot) : module(bot)
{
	name = "autofactoid";
}

void AutoFactoidModule::onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag)
{
	Stat * stat;
	{
		map<string, module *>::iterator s = bot.modhandler.modules.find("stat");
		if (s == bot.modhandler.modules.end() || s->second == NULL)
		{
			if (islogging) cout << "Autofactoid: Error: Stat module not loaded" << endl;
			return;
		}
		stat = (Stat *) s->second;
		bot.modhandler.UpdateModuleByName("stat");
	}

	if (command != "privmsg"
	|| args.length() < 1 || (args.at(0) == bot.commandprefix && args.at(0) != ':')
	|| channelwhitelist.find(tolower(target)) == channelwhitelist.end()
	|| userblacklist.find(tolower(target)) != userblacklist.end())
		return;
	vector<string> factoidcallinfo = WasFactoidCalled(args);
	if (factoidcallinfo.size() == 0)
	{
		if (islogging) cout << "User message recieved." << endl;
		string name = speaker.GetNickL();
		if (userblacklist.find(name) != userblacklist.end())
			return;
		string callfactoid = MatchLineWithDB(ToAlphaNumeric(tolower(args)));
		if (callfactoid == "" || factoidblacklist.find(callfactoid) != factoidblacklist.end())
			return;
		if (islogging) cout << "Calling factoid \"" << callfactoid << "\" to " << speaker.GetNick() << endl;
		CallFactoid(callfactoid, speaker, respondto);
	}
	else
	{
		if (islogging) cout << "Detected use of factoid: " << factoidcallinfo[0] << " toward " << factoidcallinfo[1] << endl;
		if (factoidblacklist.find(factoidcallinfo[0]) != factoidblacklist.end())
		{
			if (islogging) cout << "Factoid ignored: blacklist" << endl;
			return;
		}
		bool empty = IsEmpty(factoidcallinfo[0]);
		float VMAXweight = -9999;
		int correctindex = -1, index = 0;
		LineDB<8>& db = stat->GetUserLineDB(speaker.GetNickL());
		db.Size();
		for (int i = 0; i < 3; ++i)
		{
			float weight = GetSimilarityIndex(factoidcallinfo[0], db.GetLine(i).message);
			if (weight > VMAXweight) 
			{
				VMAXweight = weight;
				correctindex = index;
			}
			++index;
		}
		index = 0;
		for (int i = 0; i < 3; ++i)
		{
			InsertIntoDatabase(factoidcallinfo[0], db.GetLine(i).message, empty || (index == correctindex));
			++index;
		}
	}
}

void AutoFactoidModule::onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto)
{
	string largs = tolower(trim(args));
	if (controlstring == "allow" || controlstring == "deny" || controlstring == "list")
	{
		istringstream s(largs);
		s >> largs;
		if (largs.length() > 0 && largs.at(0) == '#' && controlstring == "allow")
		{
			respondto.Say("Allowed autofactoids in " + largs);
			channelwhitelist.insert(largs);
		}
		else if (largs.length() > 0 && largs.at(0) == '#' && controlstring == "deny")
		{
			respondto.Say("Disabled autofactoids in " + largs);
			channelwhitelist.erase(largs);
		}
		else if (controlstring == "list")
		{
			string s;
			for (set<string>::iterator i = channelwhitelist.begin(); i != channelwhitelist.end(); ++i)
				s += " " + *i;
			respondto.Say("Autofactoid is enabled in the following channels:" + s);
		}
	}
	else if (controlstring == "log")
	{
		islogging = (largs == "on");
		if (islogging) cout << "Enabled logging." << endl; else cout << "Disabled logging." << endl;
			respondto.Say(string("Autofactoid logging ") + (islogging ? "enabled." : "disabled."));
	}
	else if (controlstring == "add")
	{
		istringstream s(largs);
		string f, data;
		s >> f;
		data = ToAlphaNumeric(getline(s));
		InsertIntoDatabase(f, data, true);
		respondto.Say("\"" + data + "\" added to entry for \"" + f + "\".");
	}
	else if (controlstring == "remove")
	{
		istringstream s(largs);
		string f, data;
		s >> f;
		data = ToAlphaNumeric(getline(s));
		RemoveFromDatabase(f, data);
		respondto.Say("\"" + data + "\" removed from entry for \"" + f + "\".");
	}
	else if (controlstring == "purge")
	{
		istringstream s(largs);
		string f;
		s >> f;
		afdatabase.erase(tolower(f));
		respondto.Say("Entry for \"" + f + "\" purged from database.");
	}
	else if (controlstring == "dontcall")
	{
		istringstream s(largs);
		string sc, f;
		s >> sc;
		s >> f;
		if (sc == "add")
		{
			factoidblacklist.insert(f);
			respondto.Say("\"" + f + "\" will no longer be called automatically.");
		}
		else if (sc == "remove")
		{
			factoidblacklist.erase(f);
			respondto.Say("\"" + f + "\" will be called automatically again.");
		}
		else
			respondto.Say("You must specify 'add' or 'remove' and a factoid.");
	}
	else if (controlstring == "ignore")
	{
		istringstream s(largs);
		string u, sc;
		s >> sc;
		s >> u;
		if (sc == "add")
		{
			userblacklist.insert(u);
			respondto.Say("\"" + u + "\" will no longer automatically trigger factoids.");
		}
		else if (sc == "remove")
		{
			userblacklist.erase(u);
			respondto.Say("\"" + u + "\" can automatically trigger factoids again.");
		}
		else
			respondto.Say("You must specify 'add' or 'remove' and a user.");
	}
	else if (controlstring == "test")
	{
		istringstream s(largs);
		string f, line;
		s >> f;
		line = ToAlphaNumeric(getline(s));
		float si = GetSimilarityIndex(f, line);
		ostringstream output;
		output << "Match strength: " << si;
		respondto.Say(output.str());
	}
}

void AutoFactoidModule::LoadState()
{
	const int MAXSIZE = 1024;
	ifstream pfile("modules/IRCBot.autofactoid");
	string section;
	pfile >> section;
	while (pfile)
	{
		if (section == "[Channels]")
		{
			char buffer[MAXSIZE];
			pfile.getline(buffer, MAXSIZE);
			pfile.getline(buffer, MAXSIZE);
			vector<string> channels = splitcommands(trim(buffer), " ", false);
			for (int i = 0; i < channels.size(); ++i)
			{
				channelwhitelist.insert(channels[i]);
			}
		}
		else if (section == "[IgnoredUsers]")
		{
			char buffer[MAXSIZE];
			pfile.getline(buffer, MAXSIZE);
			pfile.getline(buffer, MAXSIZE);
			vector<string> ignoredusers = splitcommands(trim(buffer), " ", false);
			for (int i = 0; i < ignoredusers.size(); ++i)
			{
				userblacklist.insert(ignoredusers[i]);
			}
		}
		else if (section == "[DoNotCall]")
		{
			char buffer[MAXSIZE];
			pfile.getline(buffer, MAXSIZE);
			pfile.getline(buffer, MAXSIZE);
			vector<string> donotcall = splitcommands(trim(buffer), " ", false);
			for (int i = 0; i < donotcall.size(); ++i)
			{
				factoidblacklist.insert(donotcall[i]);
			}
		}
		else if (section == "[Factoids]")
		{
			char buffer[MAXSIZE];
			string factoid, wordstring, word;
			while(true)
			{
				pfile >> factoid;
				if (factoid == "END") break;
				while(true)
				{
					pfile.getline(buffer, MAXSIZE);
					wordstring = buffer;
					if (wordstring == "END") break;
					vector<string> words = splitcommands(wordstring, ";", true);
					if (words.size() != 3) continue;
					word = words[1];
					(afdatabase[factoid]).database[word] = WordNode::FromString(words[0], words[2]);
				}
			}
		}
		else break;
		pfile >> section;
	}
}

void AutoFactoidModule::SaveState()
{
	ofstream pfile("modules/IRCBot.autofactoid");
	pfile << "[Channels]" << endl;
	for (set<string>::iterator channel = channelwhitelist.begin(); channel != channelwhitelist.end(); channel++)
		pfile << *channel << " ";
	pfile << endl;
	pfile << "[IgnoredUsers]" << endl;
	for (set<string>::iterator channel = userblacklist.begin(); channel != userblacklist.end(); channel++)
		pfile << *channel << " ";
	pfile << endl;
	pfile << "[DoNotCall]" << endl;
	for (set<string>::iterator channel = factoidblacklist.begin(); channel != factoidblacklist.end(); channel++)
		pfile << *channel << " ";
	pfile << endl;
	pfile << "[Factoids]" << endl;
	for (map<string, FuzzyMatchDB >::iterator i = afdatabase.begin(); i != afdatabase.end(); ++i)
	{
		pfile << i->first << endl;
		for (map<string, WordNode>::iterator j = i->second.database.begin(); j != i->second.database.end(); ++j)
		{
			pfile << WordNode::ToString(j->first, j->second) << endl;
		}
		pfile << "END" << endl;
	}
	pfile << "END" << endl;
	pfile.close();
}

vector<string> AutoFactoidModule::WasFactoidCalled(string data)
{
	vector<string> result;	
	if (data.length() == 0) return result;
	if (data.at(0) != ':') return result;
	data = data.substr(1);
	vector<string> temp = splitcommands(data, " ", false);
	if (temp.size() < 3) return result;
	result.push_back(temp[0]);
	result.push_back(temp[2]);
	return result;
}

string AutoFactoidModule::MatchLineWithDB(string line)
{
	line = tolower(ToAlphaNumeric(line));
	vector<string> words = splitcommands(line, " ", false);
	string chosen = "";
	float VMAX = 0;
	for (map<string, FuzzyMatchDB>::iterator databaseentry = afdatabase.begin(); databaseentry != afdatabase.end(); ++databaseentry)
	{
		float f = databaseentry->second.MatchWithString(line);
		
		if (f > VMAX)
		{
			VMAX = f;
			chosen = (databaseentry->first);
			if (islogging) cout << "Overwriting old VMAX with: " << chosen << " (weight: " << f << ")" << endl;
		}
	}
	if (VMAX > .3)
		return chosen;
	return "";
}

float AutoFactoidModule::GetSimilarityIndex(string factoid, string line)
{
	map<string, FuzzyMatchDB>::iterator databaseentry = afdatabase.find(factoid);
	if (databaseentry == afdatabase.end()) return 0;
	return databaseentry->second.MatchWithString(line);
}

void AutoFactoidModule::InsertIntoDatabase(string factoid, string newdata, bool force)
{
	FuzzyMatchDB& databaseentry = afdatabase[factoid];

	if (!(force || (databaseentry.MatchWithString(newdata) > .1)))
		return;

	if (islogging) cout << "Adding line to database for " + factoid + ": " << newdata << endl;

	databaseentry.Add(newdata);
}

void AutoFactoidModule::RemoveFromDatabase(string factoid, string data)
{
	FuzzyMatchDB& databaseentry = afdatabase[factoid];

	if (islogging) cout << "Removing line from database: " << data << endl;

	databaseentry.Remove(data);
}

void AutoFactoidModule::CallFactoid(string factoid, Hostname speaker, Messenger respondto)
{
	((CmdFactoid *)bot.commands["factoid"])->factoidprocessor->Call("privmsg", speaker, respondto.GetTarget(), respondto.GetTarget(), ":" + factoid + " > " + speaker.GetNick(), false);
}

module * AutoFactoidModule::Factory(IRCBot& bot)
{
	return new AutoFactoidModule(bot);
}

bool AutoFactoidModule::IsEmpty(string factoid)
{
	map<string, FuzzyMatchDB>::iterator databaseentry = afdatabase.find(factoid);
	if (databaseentry == afdatabase.end()) return true;
	return (databaseentry->second.Entries() == 0);
}
