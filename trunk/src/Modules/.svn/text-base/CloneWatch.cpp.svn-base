#include <sstream>
#include <fstream>
#include <unistd.h>

#include "CloneWatch.h"
#include "Bottime.h"
#include "IRCBot.h"
#include "UserManager.h"
#include "Scheduler.h"
#include "misc.h"

extern "C" module * Factory(IRCBot& bot)
{
	return new CloneWatch(bot);
}

WatchSettings::WatchSettings()
{
	notifytype = 1;
	notify = "0";
}

NickEntry NickEntry::Dummy()
{
	static NickEntry n = {-1, "(INVALID)", "(INVALID)", set<string>()};
	return n;
}

bool NickEntry::OnChannel(Hostname& speaker)
{
	return (livenicks.find(speaker.GetNickL()) != livenicks.end());
}

void NickEntry::SetOnChannel(Hostname& speaker, bool onchannel)
{
	timestamp = Bottime::GetCurrentTimeMillis();
	if (onchannel)
		livenicks.insert(speaker.GetNickL());
	else
		livenicks.erase(speaker.GetNickL());
}

bool NickEntry::operator==(const NickEntry& other)
{
	return (host == other.host);
}

WatchedUserDB::WatchedUserDB()
{
}

void WatchedUserDB::Clean(int cutoff)
{
	unsigned long long ctime = Bottime::GetCurrentTimeMillis();
	set<string> toforget;
	for (map<string, NickEntry>::iterator j = entries.begin(); j != entries.end(); ++j)
	{
		if ((ctime - j->second.timestamp) / 1000 > cutoff)
			toforget.insert(j->first);
	}
	for (set<string>::iterator j = toforget.begin(); j != toforget.end(); ++j)
	{
		cout << "erased entry: " + *j + " (too old)" << endl;
		entries.erase(*j);
	}
}

NickEntry WatchedUserDB::Lookup(Hostname& speaker)
{
	map<string, NickEntry>::iterator i = entries.find(speaker.GetHostL());
	if (i == entries.end())
		return NickEntry::Dummy();
	return i->second;
}

void WatchedUserDB::AddUser(Hostname& speaker)
{
	NickEntry& n = entries[speaker.GetHostL()];
	n.lastnick = speaker.GetNick();
	n.host = speaker.GetHostL();
	n.timestamp = Bottime::GetCurrentTimeMillis();
}

bool WatchedUserDB::HasUser(Hostname& u)
{
	map<string, NickEntry>::iterator i = entries.find(u.GetHostL());
	if (i == entries.end())
		return false;
	return true;
}

void WatchedUserDB::UpdateNick(Hostname& speaker, string newnick, bool updatets)
{
	map<string, NickEntry>::iterator n = entries.find(speaker.GetHostL());
	if (n == entries.end())
		return;
	n->second.lastnick = newnick;
	n->second.livenicks.erase(speaker.GetNickL());
	n->second.livenicks.insert(tolower(newnick));
	if (updatets)
		n->second.timestamp = Bottime::GetCurrentTimeMillis();
}

bool WatchedUserDB::OnChannel(Hostname& speaker)
{
	map<string, NickEntry>::iterator i = entries.find(speaker.GetHostL());
	if (i == entries.end())
		return false;
	
	return i->second.OnChannel(speaker);
}

void WatchedUserDB::SetOnChannel(Hostname& speaker, bool onchannel)
{
	map<string, NickEntry>::iterator i = entries.find(speaker.GetHostL());
	if (i == entries.end())
		return;
	i->second.SetOnChannel(speaker, onchannel);
}

CloneWatch::CloneWatch(IRCBot& bot) : module(bot)
{
	pthread_mutex_init(&lock, NULL);
	Event e(300, -1, new FunctionCallAction(CloneWatch::DBManagerStub, (void *) this));
	eventID = bot.scheduler.AddEvent(e);
}

CloneWatch::~CloneWatch()
{
	pthread_mutex_lock(&lock);
	bot.scheduler.RemoveEvent(eventID);
	pthread_mutex_unlock(&lock);
	pthread_mutex_destroy(&lock);
}

void CloneWatch::LoadState()
{
	char buffer[4096];

	ifstream pfile("modules/IRCBot.clonewatch");
	pfile.getline(buffer, 4096);
	istringstream allowedchans(buffer);
	string temp;
	
	while (true)
	{
		allowedchans >> temp;
		if (!allowedchans)
			break;
		allowed.insert(temp);
	}

	while (true)
	{
		pfile.getline(buffer, 4096);
		istringstream newsettings(buffer);
		WatchSettings w;
		newsettings >> temp;
		newsettings >> w.notify;
		newsettings >> w.notifytype;
		if (!newsettings)
			break;
		settings[temp] = w;
	}

	while (true)
	{
		pfile.getline(buffer, 4096);
		istringstream altaccounts(buffer);
		string first, others;
		altaccounts >> first;
		if (!altaccounts || first == "$END")
			break;
		while (true)
		{
			altaccounts >> others;
			if (!altaccounts) break;
			RegisterAlt(first, others);
		}
	}
	pfile.close();
}

void CloneWatch::SaveState()
{
	ofstream pfile("modules/IRCBot.clonewatch");
	for (set<string>::iterator i = allowed.begin(); i != allowed.end(); ++i)
		pfile << *i << " ";
	pfile << endl;

	for (map<string, WatchSettings>::iterator i = settings.begin(); i != settings.end(); ++i)
		pfile << i->first << " " << i->second.notify << " " << i->second.notifytype << endl;
	pfile << "$END" << endl;

	set<string> finished;
	for (map<string, set<string> *>::iterator i = alts.begin(); i != alts.end(); ++i)
	{
		if (finished.find(i->first) != finished.end()) continue;
		set<string> * s = i->second;
		for (set<string>::iterator j = s->begin(); j != s->end(); ++j)
		{
			pfile << *j << " ";
			finished.insert(*j);
		}
		i = alts.find(i->first);
		pfile << endl;
	}

	pfile << "$END" << endl;
	pfile.close();
}

void * CloneWatch::DBManagerStub(void * param)
{
	((CloneWatch *) param)->CleanDB();
	return NULL;
}

void CloneWatch::CleanDB()
{
	pthread_mutex_lock(&lock);
	for (map<string, WatchedUserDB>::iterator i = database.begin(); i != database.end(); ++i)
	{
		i->second.Clean(43200);
	}
	pthread_mutex_unlock(&lock);
}

void CloneWatch::onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag)
{
	if (command == "nick")
	{
		if (target.length() > 0 && target.at(0) == ':')
			target = target.substr(1);
		UpdateDB(speaker, target, false);
	}
	else if (command == "part")
	{
		TestAddToDB(speaker, target, false);
	}
	else if (command == "kick")
	{
		string user;
		istringstream a(args);
		a >> user;
		Hostname h = bot.usermgr.GetHostname(user);
		TestAddToDB(h, target, false);
	}
	else if (command == "join")
	{
		NickEntry lastuser = CheckDB(speaker, target);
		if (lastuser.host == speaker.GetHostL() && lastuser.lastnick != speaker.GetNick())
			UpdateDB(speaker, speaker.GetNick(), true);

		string message;
		if (!(lastuser == NickEntry::Dummy()) && !KnownAlt(speaker.GetNickL(), tolower(lastuser.lastnick)))
		{
			if (lastuser.livenicks.size() != 0)
			{
				message = speaker.GetNick() + " (" + speaker.GetHost() + ") is currently in this channel as ";
				for (set<string>::iterator i = lastuser.livenicks.begin();;)
				{
					message += "'" + *i + "'";
					++i;
					if (i == lastuser.livenicks.end())
					{
						message += ".";
						break;
					}
					else
						message += ", ";
				}
			}
			else if (tolower(lastuser.lastnick) == speaker.GetNickL() && (Bottime::GetCurrentTimeMillis() - lastuser.timestamp) > 300000)
				message = speaker.GetNick() + " (" + speaker.GetHost() + ") was last here " + AsHumanReadableTime(Bottime::GetCurrentTimeMillis() - lastuser.timestamp) + " ago.";
			else if ((Bottime::GetCurrentTimeMillis() - lastuser.timestamp) > 30000)
				message = speaker.GetNick() + " (" + speaker.GetHost() + ") was last in this channel as '" + lastuser.lastnick + "' [" + AsHumanReadableTime(Bottime::GetCurrentTimeMillis() - lastuser.timestamp) + " ago]";

			if (Settings(target).notifytype == 1)
				GetMessenger(Settings(target).notify).Message(message);
			else if (Settings(target).notifytype == 2)
				GetMessenger(Settings(target).notify).Notice(message);
		}

		AddToDB(speaker, target, true);
	}
	else if (command == "quit")
		AddToAllDB(speaker, false);
}

void CloneWatch::onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto)
{
	string subject = target;
	if (args.length() != 0 && args.at(0) == '#')
	{
		istringstream s(args);
		s >> subject;
		args = trim(getline(s));
	}
	subject = tolower(subject);
	if (subject.length() == 0 || subject.at(0) != '#')
	{
		respondto.Say("You must specify a channel.");
		return;
	}
	if (controlstring == "allow")
	{
		allowed.insert(subject);
		respondto.Say("Allowing user tracking in " + subject);
	}
	else if (controlstring == "deny")
	{
		allowed.erase(subject);
		respondto.Say("Disabling user tracking in " + subject);
	}
	else if (controlstring == "responsetype")
	{
		if (args == "notice")
		{
			Settings(subject).notifytype = 2;
			respondto.Say("Response type for " + subject + " set to notice.");
		}
		else
		{
			Settings(subject).notifytype = 1;
			respondto.Say("Response type for " + subject + " set to message.");
		}
	}
	else if (controlstring == "notify")
	{
		string n = ((args == "") ? subject : args);
		Settings(subject).notify = n;
		respondto.Say("Response target for " + subject + " set to " + n + ".");
	}
	else if (controlstring == "setalt")
	{
		vector<string> nicks = splitcommands(args, " ", false);
		if (nicks.size() != 2)
		{
			respondto.Say("You must specify 2 nicks.");
			return;
		}
		respondto.Say("Registered " + nicks[1] + " as alt nick of " + nicks[0] + ".");
		RegisterAlt(tolower(nicks[0]), tolower(nicks[1]));
	}
	else if (controlstring == "remalt")
	{
		vector<string> nicks = splitcommands(args, " ", false);
		if (nicks.size() != 1)
		{
			respondto.Say("You must specify 1 nick.");
			return;
		}
		respondto.Say("Unregistered " + nicks[0] + " as alt nick.");
		UnregisterAlt(tolower(nicks[0]));
	}
}

void CloneWatch::UpdateDB(Hostname& speaker, string newnick, bool updatets)
{
	map<string, set<string> >::iterator chansofuser = bot.usermgr.channels.find(speaker.GetNick());
	if (chansofuser == bot.usermgr.channels.end())
	{
		cout << "an error occurred: a user who did not exist changed their nick!" << endl;
		return;
	}
	pthread_mutex_lock(&lock);
	for (set<string>::iterator chan = allowed.begin(); chan != allowed.end(); ++chan)
	{
		if (chansofuser->second.find(*chan) == chansofuser->second.end())
			continue;
		database[*chan].UpdateNick(speaker, newnick, updatets);
	}
	pthread_mutex_unlock(&lock);
}

void CloneWatch::AddToDB(Hostname& speaker, string channel, bool onchannel)
{
	AddToDB(speaker, database[tolower(channel)], onchannel);
}

void CloneWatch::TestAddToDB(Hostname& speaker, string channel, bool onchannel)
{
	pthread_mutex_lock(&lock);
	WatchedUserDB& w = database[tolower(channel)];
	AddToDB(speaker, w, onchannel);
	if (w.HasUser(speaker))
	{
		w.SetOnChannel(speaker, onchannel);
	}
	pthread_mutex_unlock(&lock);
}

void CloneWatch::AddToDB(Hostname& speaker, WatchedUserDB& chandb, bool onchannel)
{
	chandb.AddUser(speaker);
	chandb.SetOnChannel(speaker, onchannel);
}

void CloneWatch::AddToAllDB(Hostname& speaker, bool onchannel)
{
	pthread_mutex_lock(&lock);
	for (set<string>::iterator chan = allowed.begin(); chan != allowed.end(); ++chan)
	{
		map<string, WatchedUserDB>::iterator chandb = database.find(*chan);
		if (chandb == database.end())
			continue;
		if (!chandb->second.OnChannel(speaker))
			continue;
		AddToDB(speaker, chandb->second, onchannel);
	}
	pthread_mutex_unlock(&lock);
}

WatchSettings& CloneWatch::Settings(string s)
{
	return settings[tolower(s)];
}

NickEntry CloneWatch::CheckDB(Hostname& speaker, string channel)
{
	pthread_mutex_lock(&lock);
	map<string, WatchedUserDB>::iterator chandb = database.find(tolower(channel));
	if (chandb == database.end())
	{
		pthread_mutex_unlock(&lock);
		return NickEntry::Dummy();
	}
	NickEntry n = chandb->second.Lookup(speaker);
	pthread_mutex_unlock(&lock);
	return n;
}

void CloneWatch::Dump(NickEntry n)
{
	cout << "Entry for " << n.host << ":" << endl;
	cout << "Nast nick: " << n.lastnick << endl;
	cout << "Currently on channel as:";
	for (set<string>::iterator i = n.livenicks.begin(); i != n.livenicks.end(); ++i) cout << " " << *i;
	cout << endl;
}

bool CloneWatch::KnownAlt(string nick1, string nick2)
{
	map<string, set<string> *>::iterator n1 = alts.find(nick1), n2 = alts.find(nick2);
	if (n1->second == n2->second && n1 != alts.end())
		return true;
	return false;
}

void CloneWatch::RegisterAlt(string nick1, string nick2)
{
	set<string> *& n1 = alts[nick1], *& n2 = alts[nick2];
	if (n1 == NULL && n2 == NULL)
	{
		n1 = new set<string>();
		n2 = n1;
		n1->insert(nick1); n1->insert(nick2);
	}
	else if (n1 != NULL && n2 != NULL && n1 != n2)
	{
		set<string> * p = n2;
		
		for (set<string>::iterator i = n2->begin(), e = n2->end(); i != e; ++i)
		{
			n1->insert(*i);
			alts[*i] = n1;
		}
		delete p;
	}
	else if (n1 != NULL && n2 == NULL || n1 == NULL && n2 != NULL)
	{
		set<string> * p = (n1 ? n1 : n2);
		p->insert(nick1);
		p->insert(nick2);
		n1 = p;
		n2 = p;
	}
}

void CloneWatch::UnregisterAlt(string nick1)
{
	set<string> * n1 = alts[nick1];
	if (n1 != NULL)
	{
		n1->erase(nick1);
		if (n1->size() == 1)
		{
			alts.erase(*(n1->begin()));
			delete n1;
		}
	}
	alts.erase(nick1);
	
}
