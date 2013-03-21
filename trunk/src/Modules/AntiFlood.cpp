#include <fstream>
#include <sstream>

#include "AntiFlood.h"
#include "FloodEngine.h"

#include "IRCBot.h"
#include "Scheduler.h"
#include "EventActions.h"
#include "UserManager.h"
#include "misc.h"

extern "C" module * Factory(IRCBot& bot)
{
	return new FloodProtectModule(bot);
}

ChannelSettings::ChannelSettings()
{
	for (int i = 0; i < FloodProtectModule::N_ACTIONS; ++i)
	{
		reaction[i] = "";
		cleanup[i] = "";
		timeout[i] = 0;
	}

	for (int i = 0; i < FloodProtectModule::N_TYPES; ++i)
	{
		msg_threshold[i] = 4;
		time_threshold[i] = 7.0;
	}
}

map<string, FloodType> FloodProtectModule::floodtypenames = FloodProtectModule::GetFloodTypeNames();

map<string, FloodType> FloodProtectModule::GetFloodTypeNames()
{
	map<string, FloodType> m;
	m["message"] = F_MSG;
	m["msg"] = F_MSG;
	m["nickname"] = F_NICK;
	m["nick"] = F_NICK;
	m["cycle"] = F_CYCLE;
	m["part"] = F_CYCLE;
	m["ping"] = F_PING;
	m["highlight"] = F_PING;
	m["channel"] = F_CHAN;
	m["chan"] = F_CHAN;
	return m;
}

FloodProtectModule::FloodProtectModule(IRCBot& bot) : module(bot),
						      nickhandler(new NickFloodEngine()),
						      cyclehandler(new CycleFloodEngine()),
						      msghandler(new MessageFloodEngine()),
						      pinghandler(new PingFloodEngine()),
						      chanhandler(new ChannelFloodEngine())
{
	pruneevent = bot.scheduler.AddEvent(Event(300, -1, new FunctionCallAction(FloodProtectModule::Prune, this)));
	pthread_mutex_init(&dblock, NULL);
}

void FloodProtectModule::DBLock()
{
	pthread_mutex_lock(&dblock);
}

void FloodProtectModule::DBUnlock()
{
	pthread_mutex_unlock(&dblock);
}

FloodProtectModule::~FloodProtectModule()
{
	DBLock();
	bot.scheduler.RemoveEvent(pruneevent);
	DBUnlock();
	pthread_mutex_destroy(&dblock);
	delete nickhandler;
	delete cyclehandler;
	delete msghandler;
	delete pinghandler;
	delete chanhandler;
}

void FloodProtectModule::LoadState()
{
	char buffer[4096];
	ifstream pfile("modules/IRCBot.floodprotect");
	string line;
	string token;
	ChannelSettings * currentsettings = NULL;
	while (true)
	{
		pfile >> token;
		cout << "section: " << token << endl;
		if (!pfile) break;
		if (token == "[EnabledIn]")
		// format: [EnabledIn] #channel1 #channel2 #etcetera
		{
			pfile.getline(buffer, 4096);
			istringstream c(buffer);
			string t;
			while (true)
			{
				c >> t; t = tolower(t);
				if (!c) break;
				//cout << "EnableIn: " << t << endl;
				channels.insert(t);
			}
		}
		else if (token == "[Channel]")
		// format: [Channel] #dorpenborp
		{
			pfile.getline(buffer, 4096);
			istringstream c(buffer);
			string t;
			c >> t; t = tolower(t);
			//cout << "Channel: " << t << endl;
			if (c) currentsettings = &(settings[t]);
		}
		else if (token == "[Immune]")
		// format: [Immune] Wug TheBadShepperd xales GreyVulpine Kramer Sqozza _303 etcetera
		{
			pfile.getline(buffer, 4096);
			istringstream c(buffer);
			string t;
			while (true && currentsettings != NULL)
			{
				c >> t; t = tolower(t);
				if (!c) break;
				//cout << "Immune: " << t << endl;
				currentsettings->immune.insert(t);
			}
		}
		else if (token == "[Timeouts]")
		// format: [Timeouts] i i i i i i i i i i (i is an integer value)
		// exactly 10 floats, rest is ignored
		{
			pfile.getline(buffer, 4096);
			istringstream c(buffer);
			int t;
			for (int i = 0; i < FloodProtectModule::N_ACTIONS && currentsettings != NULL; ++i)
			{
				c >> t;
				if (c) currentsettings->timeout[i] = t;
				else currentsettings->timeout[i] = 300;
				//cout << "Timeouts: " << currentsettings->timeout[i] << endl;
			}
		}
		else if (token == "[MsgThresholds]")
		// format: [Thresholds] i i i i i (i is an integer value)
		{
			pfile.getline(buffer, 4096);
			istringstream c(buffer);
			int t;
			for (int i = 0; i < FloodProtectModule::N_TYPES && currentsettings != NULL; ++i)
			{
				c >> t;
				if (c && t >= 0 && t < 11) currentsettings->msg_threshold[i] = t;
				else currentsettings->msg_threshold[i] = 4;
				//cout << "Thresholds: " << currentsettings->timeout[i] << endl;
			}
		}
		else if (token == "[TimeThresholds]")
		// format: [Thresholds] f f f f f (f is a floating point value)
		{
			pfile.getline(buffer, 4096);
			istringstream c(buffer);
			float t;
			for (int i = 0; i < FloodProtectModule::N_TYPES && currentsettings != NULL; ++i)
			{
				c >> t;
				if (c && t >= 0 && t <= 60) currentsettings->time_threshold[i] = t;
				else currentsettings->time_threshold[i] = 7.0;
				//cout << "Thresholds: " << currentsettings->timeout[i] << endl;
			}
		}
		else if (token == "[Reaction]")
		// format: [Reaction] num command goes here
		// num is the index number, 0 to 9.  rest is a command
		{
			pfile.getline(buffer, 4096);
			istringstream c(buffer);
			int i = 0, t;
			c >> t;
			if (c && t >= 0 && t < FloodProtectModule::N_ACTIONS && currentsettings != NULL) currentsettings->reaction[t] = trim(getline(c));
			//if (t >= 0 && t < ChannelSettings::N_ACTIONS) cout << "Reaction " << t << ": " << currentsettings->reaction[t] << endl;
		}
		else if (token == "[Cleanup]")
		// format: [Cleanup] num command goes here
		// num is the index number, 0 to 9.  rest is a command
		{
			pfile.getline(buffer, 4096);
			istringstream c(buffer);
			int i = 0, t;
			c >> t;
			if (c && t >= 0 && t < FloodProtectModule::N_ACTIONS && currentsettings != NULL) currentsettings->cleanup[t] = trim(getline(c));
			//if (t >= 0 && t < ChannelSettings::N_ACTIONS) cout << "Cleanup " << t << ": " << currentsettings->cleanup[t] << endl;
		}
	}
	pfile.close();
}

void FloodProtectModule::SaveState()
{
	ofstream pfile("modules/IRCBot.floodprotect", ios_base::trunc);
	pfile << "[EnabledIn]";
	for (set<string>::iterator chan = channels.begin(); chan != channels.end(); ++chan)
		pfile << " " << *chan;
	pfile << endl;
	for (map<string, ChannelSettings>::iterator chan = settings.begin(); chan != settings.end(); ++chan)
	{
		pfile << "[Channel] " << chan->first << endl << "[Immune]";
		for (set<string>::iterator u = chan->second.immune.begin(); u != chan->second.immune.end(); ++u)
			pfile << " " << *u;
		pfile << endl << "[Timeouts]";
		for (int i = 0; i < N_ACTIONS; ++i)
			pfile << " " << chan->second.timeout[i];
		pfile << endl << "[MsgThresholds]";
		for (int i = 0; i < N_TYPES; ++i)
			pfile << " " << chan->second.msg_threshold[i];
		pfile << endl << "[TimeThresholds]";
		for (int i = 0; i < N_TYPES; ++i)
			pfile << " " << chan->second.time_threshold[i];
		pfile << endl;
		for (int i = 0; i < N_ACTIONS; ++i)
			pfile << "[Reaction] " << i << " " << chan->second.reaction[i] << endl;
		for (int i = 0; i < N_ACTIONS; ++i)
			pfile << "[Cleanup] " << i << " " << chan->second.cleanup[i] << endl;
	}
	pfile.close();
}

// called synchronously by event thread
// should visit all of the databases and get rid of any entries that are too old to be considered
void * FloodProtectModule::Prune(void * self)
{
	FloodProtectModule * me = (FloodProtectModule *) self;
	me->DoPrune();
	return NULL;
}

void FloodProtectModule::DoPrune()
{
	DBLock();

	mtime_t cutoff = Bottime::GetCurrentTimeMillis() - 300000LL;

	map<string, UserInfoCollector>::iterator i = userevents.begin();
	while (i != userevents.end())
	{
		if ((cutoff - i->second.lastupdate) > 0)
			userevents.erase(i++);
		else
			++i;
	}

	map<string, deque<ActionEvent> >::iterator j = actioneventsbychannel.begin();
	while (j != actioneventsbychannel.end())
	{
		if (j->second.size() == 0 || (cutoff - j->second[0].timestamp) > 0)
			actioneventsbychannel.erase(j++);
		else
			++j;
	}

	DBUnlock();
}

string FloodProtectModule::GetCommandString(string cmdstring, Hostname& speaker, string target, string reason)
{
	return 		stringreplace(
			stringreplace(
			stringreplace(
			stringreplace(
			stringreplace(
			stringreplace(		cmdstring, "%%", "\t")
						, "%n", speaker.GetNick())
						, "%r", reason)
						, "%t", target)
						, "%h", speaker.GetHost())
						, "\t", "%");
}

void FloodProtectModule::PerformCommand(string cmdstring, Hostname& speaker, string target, string reason)
{
	cmdstring = GetCommandString(cmdstring, speaker, target, reason);
	bot.PerformCommand(speaker, target, target, cmdstring);
}

void FloodProtectModule::PerformCommandFuture(float timeout, string cmdstring, Hostname& speaker, string target, string reason)
{
	cmdstring = GetCommandString(cmdstring, speaker, target, reason);
	Event e(timeout, new BotCommandAction(bot, cmdstring, speaker, target));
	bot.scheduler.AddEvent(e);
}

void FloodProtectModule::onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag)
{
	string ltarget = tolower(target);
	mtime_t now = Bottime::GetCurrentTimeMillis();

	DBLock();
	do
	{
		if (command == "privmsg" || command == "notice")
		{
			if (channels.find(ltarget) == channels.end()) break;
			if (settings[ltarget].immune.find(speaker.GetNickL()) != settings[ltarget].immune.end()) break;
			if (settings[ltarget].msg_threshold[F_MSG] != userevents[speaker.GetNick()].message[ltarget].MaxSize()) userevents[speaker.GetNick()].message[ltarget].Reset(60000, settings[ltarget].msg_threshold[F_MSG]);
			userevents[speaker.GetNick()].AddMsg(ltarget, args);
			msghandler->Analyze(this, speaker, target, respondto, bot);
			pinghandler->Analyze(this, speaker, target, respondto, bot);
			chanhandler->Analyze(this, speaker, target, respondto, bot);
		}
		else if (command == "part")
		{
			if (channels.find(ltarget) == channels.end()) break;
			if (settings[ltarget].immune.find(speaker.GetNickL()) != settings[ltarget].immune.end()) break;
			if (settings[ltarget].msg_threshold[F_CYCLE] != userevents[speaker.GetNick()].leave[ltarget].MaxSize()) userevents[speaker.GetNick()].leave[ltarget].Reset(60000, settings[ltarget].msg_threshold[F_CYCLE]);
			userevents[speaker.GetNick()].AddLeave(ltarget);
			cyclehandler->Analyze(this, speaker, target, respondto, bot);
			chanhandler->Analyze(this, speaker, target, respondto, bot);
		}
		else if (command == "nick")
		{
			userevents[speaker.GetNick()].AddNick();
			userevents[target] = userevents[speaker.GetNick()]; // if NICK, target is new nickname so preserve case

			Hostname newspeaker = speaker;
			newspeaker.SetNick(target);

			set<string>& userchannels = bot.usermgr.channels[speaker.GetNick()];

			// yucky for loopy stuff over every channel protection is active in AND the user is in
			for (set<string>::iterator channel = channels.begin(), chend = channels.end(); channel != chend; ++channel)
			{
				string lchannel = tolower(*channel);
				if (userchannels.find(lchannel) == userchannels.end()) continue;
				if (	(settings.find(lchannel) != settings.end()) &&
					(settings[lchannel].immune.find(newspeaker.GetNickL()) != settings[lchannel].immune.end()))
					continue;
				nickhandler->Analyze(this, newspeaker, *channel, respondto, bot);
				chanhandler->Analyze(this, newspeaker, *channel, respondto, bot);
			}
		}
	} while (false); // just so I can break out of it
	DBUnlock();
}

void FloodProtectModule::onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto)
{
	string subject = target;
	if (args.length() != 0 && args.at(0) == '#')
	{
		istringstream s(args);
		s >> subject;
		args = trim(getline(s));
	}
	subject = tolower(subject);

	if (controlstring == "enable")
	{
		if (subject.length() > 0 && subject[0] == '#')
		{
			channels.insert(subject);
			respondto.Say("Added channel: " + subject);
		}
		else
			respondto.Say("Couldn't add channel: did you forget to specify it?");
	}
	else if (controlstring == "disable")
	{
		if (channels.erase(subject))
			respondto.Say("Removed channel: " + subject);
		else
			respondto.Say("Couldn't remove channel: did you forget to specify it?");
	}
	else // 2 part command
	{
		istringstream s(args);
		string subcommand = "[BLANK]", id = "[BLANK]";
		s >> subcommand;
		s >> id;
		subcommand = tolower(subcommand);
		id = tolower(id);

		if ((subcommand == "get" || subcommand == "test") && settings.find(subject) == settings.end())
		{
			respondto.Say("No settings exist for this channel.");
			return;
		}

		if (controlstring == "warning") // set, get, test
		{
			int index = -1;
			if (floodtypenames.find(id) == floodtypenames.end())
			{
				respondto.Say("Unrecognized flood type: " + id);
				return;
			}
			else
				index = (int) floodtypenames[id];
			if (subcommand == "get")
				respondto.Say("Response: " + settings[subject].reaction[index]);
			else if (subcommand == "set")
			{
				settings[subject].reaction[index] = trim(getline(s));
				respondto.Say("Done.");
			}
			else if (subcommand == "test")
				PerformCommand(settings[subject].reaction[index], speaker, target, "Flood Test (" + id + ")");
		}
		else if (controlstring == "penalty") // set, get, test
		{
			int index = -1;
			if (floodtypenames.find(id) == floodtypenames.end())
			{
				respondto.Say("Unrecognized flood type: " + id);
				return;
			}
			else
				index = (int) floodtypenames[id] + N_TYPES;
			if (subcommand == "get")
				respondto.Say("Response: " + settings[subject].reaction[index]);
			else if (subcommand == "set")
			{
				settings[subject].reaction[index] = trim(getline(s));
				respondto.Say("Done.");
			}
			else if (subcommand == "test")
				PerformCommand(settings[subject].reaction[index], speaker, target, "Flood Test (" + id + ")");
		}
		else if (controlstring == "warningtimeout") // set, get, test
		{
			int index = -1;
			if (floodtypenames.find(id) == floodtypenames.end())
			{
				respondto.Say("Unrecognized flood type: " + id);
				return;
			}
			else
				index = (int) floodtypenames[id];
			if (subcommand == "get")
				respondto.Say("Response: " + settings[subject].cleanup[index]);
			else if (subcommand == "set")
			{
				settings[subject].cleanup[index] = trim(getline(s));
				respondto.Say("Done.");
			}
			else if (subcommand == "test")
				PerformCommand(settings[subject].cleanup[index], speaker, target, "Flood Test (" + id + ")");
		}
		else if (controlstring == "penaltytimeout") // set, get, test
		{
			int index = -1;
			if (floodtypenames.find(id) == floodtypenames.end())
			{
				respondto.Say("Unrecognized flood type: " + id);
				return;
			}
			else
				index = (int) floodtypenames[id] + N_TYPES;
			if (subcommand == "get")
				respondto.Say("Response: " + settings[subject].cleanup[index]);
			else if (subcommand == "set")
			{
				settings[subject].cleanup[index] = trim(getline(s));
				respondto.Say("Done.");
			}
			else if (subcommand == "test")
				PerformCommand(settings[subject].cleanup[index], speaker, target, "Flood Test (" + id + ")");
		}
		else if (controlstring == "warningduration") // set, get, test
		{
			int index = -1;
			if (floodtypenames.find(id) == floodtypenames.end())
			{
				respondto.Say("Unrecognized flood type: " + id);
				return;
			}
			else
				index = (int) floodtypenames[id];
			if (subcommand == "get")
				respondto.Say("Duration: " + tostr(settings[subject].timeout[index]));
			else if (subcommand == "set")
			{
				float newduration;
				s >> newduration;
				if (!s)
				{
					respondto.Say("Invalid numeric value.");
					return;
				}
				settings[subject].timeout[index] = newduration;
				respondto.Say("Done.");
			}
		}
		else if (controlstring == "penaltyduration") // set, get, test
		{
			int index = -1;
			if (floodtypenames.find(id) == floodtypenames.end())
			{
				respondto.Say("Unrecognized flood type: " + id);
				return;
			}
			else
				index = (int) floodtypenames[id] + N_TYPES;
			if (subcommand == "get")
				respondto.Say("Duration: " + tostr(settings[subject].timeout[index]));
			else if (subcommand == "set")
			{
				float newduration;
				s >> newduration;
				if (!s)
				{
					respondto.Say("Invalid numeric value.");
					return;
				}
				settings[subject].timeout[index] = newduration;
				respondto.Say("Done.");
			}
		}
		else if (controlstring == "threshold" || controlstring == "tolerance") // set, get
		{
			int index = -1;
			if (floodtypenames.find(id) == floodtypenames.end())
			{
				respondto.Say("Unrecognized flood type: " + id);
				return;
			}
			else
				index = (int) floodtypenames[id];
			if (subcommand == "get")
				respondto.Say("Tolerance: " + tostr(settings[subject].time_threshold[index]) + " " + tostr(settings[subject].msg_threshold[index]));
			else if (subcommand == "set")
			{
				float newtol_time;
				int newtol_msg;
				s >> newtol_time;
				s >> newtol_msg;
				if (!s || newtol_msg < 0 || newtol_msg > 10 || newtol_time < 0 || newtol_time > 60)
				{
					respondto.Say("Invalid numeric value or values.");
					return;
				}
				settings[subject].time_threshold[index] = newtol_time;
				settings[subject].msg_threshold[index] = newtol_msg;
				respondto.Say("Done.");
			}
		}
		else if (controlstring == "immune") // add, remove, clear
		{
			if (subcommand == "add")
			{
				settings[subject].immune.insert(id);
				respondto.Say("Added to immune list: " + id);
			}
			else if (subcommand == "remove")
			{
				settings[subject].immune.erase(id);
				respondto.Say("Removed from immune list: " + id);
			}
			else if (subcommand == "clear")
			{
				settings[subject].immune.clear();
				respondto.Say("Immune list cleared.");
			}
		}
	}
	return;
}

// don't even fucking THINK of calling this unless you have called DBLock() first
void FloodProtectModule::AddChannelEvent(string name, ActionEvent event)
{
	deque<ActionEvent>& derp = actioneventsbychannel[name];
	derp.push_front(event);
	if (derp.size() > MAX_CHAN_EVENTS)
	{
		derp.pop_back();
	}
}
