#include <string>
#include <fstream>

#include "FactoidProcessor.h"
#include "IRCBot.h"
#include "IRC.h"
#include "BotPermissions.h"
#include "misc.h"

using namespace std;

int FactoidProcessor::AddFactoid(string factoid, string data, Hostname caller, string target, string respondto)
{
	factoid = tolower(factoid);
	if (blockedchannels.find(tolower(target)) != blockedchannels.end())
		return CHANBANNED;
	if (HasFactoid(factoid))
		return BADSTATE;
	if (!HasAccess(caller, factoid))
		return LOCKED;
	if (factoid != "")
	{
		factoids[factoid] = data;
		setters[factoid] = caller.GetAccountL();
	}
	return OK;
}

int FactoidProcessor::AppendFactoid(string factoid, string newdata, Hostname caller, string target, string respondto)
{
	factoid = tolower(factoid);
	if (blockedchannels.find(tolower(target)) != blockedchannels.end())
		return CHANBANNED;
	if (!HasFactoid(factoid))
		return BADSTATE;
	if (!HasAccess(caller, factoid))
		return LOCKED;
	string& data = factoids[factoid];
	if (!(newdata.length() <= 1 || data.length() <= 1 || newdata.substr(0, 2) == ";;" || data.substr(data.length() - 2, 2) == ";;" || (newdata.at(0) == ';' && data.at(data.length() - 1) == ';')))
		data.append(" ");
	factoids[factoid].append(newdata);
	setters[factoid] = caller.GetAccountL();
	return OK;
}

int FactoidProcessor::RemoveFactoid(string factoid, Hostname caller, string target, string respondto)
{
	factoid = tolower(factoid);
	if (blockedchannels.find(tolower(target)) != blockedchannels.end())
		return CHANBANNED;
	if (!HasFactoid(factoid))
		return BADSTATE;
	if (!HasAccess(caller, factoid))
		return LOCKED;
	factoids.erase(factoid);
	setters.erase(factoid);
	locklevels.erase(factoid);
	return OK;
}

int FactoidProcessor::ModifyFactoid(string factoid, string newdata, Hostname caller, string target, string respondto)
{
	factoid = tolower(factoid);
	if (factoid == "")
		return RemoveFactoid(factoid, caller, target, respondto);
	if (blockedchannels.find(tolower(target)) != blockedchannels.end())
		return CHANBANNED;
	if (!HasFactoid(factoid))
		return BADSTATE;
	if (!HasAccess(caller, factoid))
		return LOCKED;
	factoids[factoid] = newdata;
	setters[factoid] = caller.GetAccountL();
	return OK;
}

int FactoidProcessor::Enable(string location)
{
	if (blockedchannels.erase(location) == 1)
		return OK;
	else
		return BADSTATE;
}

int FactoidProcessor::Disable(string location)
{
	bool b = blockedchannels.find(location) == blockedchannels.end();
	blockedchannels.insert(location);
	if (b)
		return OK;
	else
		return BADSTATE;
}

int FactoidProcessor::Ban(string user)
{
	bool b = bannedusers.find(user) == bannedusers.end();
	bannedusers.insert(user);
	if (b)
		return OK;
	else
		return BADSTATE;
}

int FactoidProcessor::Unban(string user)
{
	if (bannedusers.erase(user) == 1)
		return OK;
	else
		return BADSTATE;
}

int FactoidProcessor::Lock(string factoid, int locklevel)
{
	map<string, int>::iterator i = locklevels.find(factoid);
	if (i != locklevels.end() && i->second != 0)
		return BADSTATE;
	if (locklevel == 0)
		return LOCKED;
	locklevels[factoid] = locklevel;
	return OK;
}

int FactoidProcessor::Unlock(string factoid, int unlocklevel)
{
	map<string, int>::iterator i = locklevels.find(factoid);
	if (i == locklevels.end() || i->second == 0)
		return BADSTATE;
	if (unlocklevel < i->second)
		return LOCKED;
	locklevels.erase(factoid);
	return OK;
}

bool FactoidProcessor::Call(string command, Hostname speaker, string target, string respondto, string message, bool c)
{
	if (command != "privmsg")
		return false;
	if (message.length() == 0)
		return false;
	if (bannedusers.find(speaker.GetAccountL()) != bannedusers.end())
		return false;

	bool isquery;
	string factoidname;
	string channelname = tolower(target);
	string specifiedtarget = target;
	string rtype = "";

	if (message.find(":?") == 0 || message.find("~?") == 0)
	{
		isquery = true;
		istringstream ch(message);
		ch >> factoidname;
		factoidname = tolower(factoidname.substr(2));
	}
	else if (message.find(":") == 0 || message.find("~") == 0)
	{
		isquery = false;
		istringstream ch(message);
		ch >> factoidname;
		if (ch)
			ch >> rtype;
		if (ch)
			ch >> specifiedtarget;
		factoidname = tolower(factoidname.substr(1));
	}

	bool canspeakeruse = (bannedusers.find(speaker.GetNickL()) == bannedusers.end() && bot.permission.CanBeUsedBy(speaker));
	bool cansendtotarget = blockedchannels.find(tolower(specifiedtarget)) == blockedchannels.end() && bot.permission.CanSendTo(specifiedtarget);
	bool cansendtochannel = blockedchannels.find(tolower(target)) == blockedchannels.end() && bot.permission.CanSendTo(target);

	if (!canspeakeruse)
		return false;

	string botresponse = GetFactoidMessage(factoidname, rtype, speaker, channelname, specifiedtarget, cansendtotarget, cansendtochannel, isquery);

	if (botresponse.length() != 0)
		bot.Raw(botresponse);
	return false;
}

string FactoidProcessor::GetFactoidMessage(string factoidname, string rtype, Hostname& speaker, string channel, string specifiedtarget, bool cansendtotarget, bool cansendtochannel, bool isquery)
{
	string respondto = (tolower(channel) == bot.lnick) ? speaker.GetNick() : channel;
	string factoidmessage = "";
	if (isquery)
	{
		if (factoidname == "")
			return "";
		string message;
		if (HasFactoid(factoidname))
		{
			message = "'" + factoidname + "' was set by: " + setters[factoidname];
			map<string, int>::iterator lock = locklevels.find(factoidname);
			if (lock != locklevels.end() && lock->second != 0)
				message += " [locked with level " + tostr(lock->second) + "]";
		}
		else
			message = factoidname + " is not set.";
		if (cansendtochannel)
			factoidmessage = IRC::Message(channel, message);
		else
			factoidmessage = IRC::Message(speaker.GetNick(), message);
		return factoidmessage;
	}
	else
	{
		if (HasFactoid(factoidname))
		{
			int c = 0;
			vector<string> f = splitcommands(factoids[factoidname], ";;", false);
			for (vector<string>::iterator i = f.begin(); i != f.end() && (c == 0 || c < linelimit); i++)
			{
				++c;
				if (rtype == "" && cansendtotarget)
					factoidmessage += IRC::Message(respondto, factoidname + ": " + *i);
				else if (rtype == "" && !cansendtotarget)
					factoidmessage += IRC::Message(speaker.GetNick(), factoidname + ": " + *i);
				else if (rtype == ">" && cansendtochannel && cansendtotarget)
					factoidmessage += IRC::Message(respondto, specifiedtarget + ": (" + factoidname + ") " + *i);
				else if ((rtype == ">" && !cansendtochannel && cansendtotarget) || (rtype == ">>" && cansendtotarget))
					factoidmessage += IRC::Message(specifiedtarget, speaker.GetNick() + " sent: (" + factoidname + ") " + *i);
				else if (rtype == "}" && cansendtochannel && cansendtotarget)
					factoidmessage += IRC::Notice(respondto, specifiedtarget + ": (" + factoidname + ") " + *i);
				else if ((rtype == "}" && !cansendtochannel && cansendtotarget) || (rtype == "}}" && cansendtotarget))
					factoidmessage += IRC::Notice(specifiedtarget, speaker.GetNick() + " sent: (" + factoidname + ") " + *i);
			}
		}
	}
	return factoidmessage;
}

bool FactoidProcessor::HasFactoid(string factoid)
{
	bool b = (factoids.find(factoid) != factoids.end());
	return b;
}

bool FactoidProcessor::HasAccess(Hostname user, string factoid)
{
	map<string, int>::iterator f = locklevels.find(factoid);
	if (f == locklevels.end())
		return true;
	string a = user.GetAccountL();
	return (bot.permission.GetOwner() == a) || (bot.permission.GetUserLevel(a) >= f->second);
}

void FactoidProcessor::Search(vector<string>& result, string query)
{
	query = tolower(query);
	for (map<string, string>::iterator i = factoids.begin(); i != factoids.end(); ++i)
	{
		if (tolower(i->first).find(query) != string::npos || tolower(i->second).find(query) != string::npos)
			result.push_back(i->first);
	}
}

void FactoidProcessor::LoadFactoidDB()
{
	ifstream pfile("IRCBot.factoids");
	pfile >> linelimit;
	int maxsize = 1024;
	int locklvl;
	string factoid, owner, data;
	char buffer[maxsize];
	while (true)
	{
		pfile >> factoid;
		pfile >> owner;
		pfile >> locklvl;
		pfile.getline(buffer, maxsize);
		if (!pfile)
			break;
		data = trim(buffer);
		factoids[factoid] = data;
		setters[factoid] = owner;
		if (locklvl != 0)
			locklevels[factoid] = locklvl;
	}
	pfile.close();
}

void FactoidProcessor::SaveFactoidDB()
{
	ofstream pfile("IRCBot.factoids");
	pfile << linelimit << endl;
	map<string, string>::iterator data = factoids.begin();
	map<string, string>::iterator set, send = setters.end();
	map<string, int>::iterator lvl, lend = locklevels.end();

	int level;
	string owner;

	for (; data != factoids.end(); ++data)
	{
		set = setters.find(data->first);
		lvl = locklevels.find(data->first);
		if (set == send)
			owner = bot.permission.GetOwner();
		else
			owner = set->second;
		if (lvl == lend)
			level = 0;
		else
			level = lvl->second;
		pfile << data->first << " ";
		pfile << owner << " ";
		pfile << level << " ";
		pfile << data->second << endl;
	}
	pfile.close();
}
