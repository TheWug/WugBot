#include "IRCProcessor.h"

#include <exception>

#include "BotCore.h"
#include "StringUtil.h"
#include "CommonEvents.h"


IRCProcessor::IRCSender::IRCSender()
{
}

IRCProcessor::IRCSender::IRCSender(string full_mask)
{
	Set(full_mask);
}

void IRCProcessor::IRCSender::Set(string full_mask)
{
	int i, j, x;
	if (full_mask == "") return;

	x = full_mask.find(':');
	i = full_mask.find('!');
	j = full_mask.find('@');

	if (i != -1) nickname = full_mask.substr(0, i);
	if (i != -1 && j != -1) ident = full_mask.substr(i + 1, j - i - 1);
	if (j != -1) hostname = full_mask.substr(j + 1);

	if (nickname == "") nickname = full_mask;
	if (ident == "") ident = full_mask;
	if (hostname == "") hostname = full_mask;
}

string IRCProcessor::IRCSender::Get()
{
	return nickname + '!' + ident + '@' + hostname;
}

IRCProcessor::IRCProcessor(BotCore& b) : bot(b)
{
}

void IRCProcessor::WelcomeHandler(void *s, EventHandler::Event e)
{
	IRCProcessor *self = (IRCProcessor *) s;
	EventHandler::Event out = e;
	out.name = CommonEvents::IRC_SEND;
	if (e.name == CommonEvents::IRC_WELCOME)
	{
		string section = "NETWORK:" + StringUtil::ToUpper(e.data[0]);
		out.data.resize(5);
		string nickname = self->bot.BotProp().GetProperty("nickname", section, "UnconfiguredBot");
		string nickserv_name = self->bot.BotProp().GetProperty("nickserv_username", section, nickname);
		string nickserv_pass = self->bot.BotProp().GetProperty("nickserv_password", section, "");
		string ns_id_method = self->bot.BotProp().GetProperty("nickserv_identify_command", section, "NS IDENTIFY %u %p");
		if (nickserv_name == "" || nickserv_pass == "")
		{
			self->bot.BotLog().GetLog(BotLogger::SYS).Put(WARNING, "IRCProcessor::WelcomeHandler: no services identification specified (not logging into services)");
			return;
		}
		vector<string> finds; finds.push_back("%u"); finds.push_back("%p");
		vector<string> replaces; replaces.push_back(nickserv_name); replaces.push_back(nickserv_pass);
		self->bot.BotLog().GetLog(BotLogger::SYS).Put(INFO, "IRCProcessor::WelcomeHandler: identifying to services as " + nickserv_name);
		ns_id_method = StringUtil::Replace(ns_id_method, finds, replaces);
		out.data.resize(2);
		out.data[1] = ns_id_method;
		self->bot.BotEvents().Raise(out);
	}
	else if (e.name == CommonEvents::NET_STARTED)
	{
		string section = "NETWORK:" + StringUtil::ToUpper(e.data[1]);
		string nickname = self->bot.BotProp().GetProperty("nickname", section, "UnconfiguredBot");
		string ident = self->bot.BotProp().GetProperty("ident", section, "PlzKill");
		string hostname = self->bot.BotProp().GetProperty("hostname", section, "*");
		string realname = self->bot.BotProp().GetProperty("realname", section, "WugBot IRC Bot v2.0 (Unconfigured)");
		out.data.resize(3);
		out.data[1] = "NICK"; out.data[2] = nickname;
		self->bot.BotEvents().Raise(out);
		out.data.resize(6);
		out.data[1] = "USER"; out.data[2] = ident; out.data[3] = hostname; out.data[4] = "*"; out.data[5] = realname;
		self->bot.BotEvents().Raise(out);
	}
}

void IRCProcessor::IOHandler(void *s, EventHandler::Event e)
{
	IRCProcessor *self = (IRCProcessor *) s;
	if (e.name == CommonEvents::IRC_SEND)
	{
		try
		{
			EventHandler::Event to_net;
			to_net.name = CommonEvents::NET_SEND;
			to_net.data.push_back(e.data[0]);
			to_net.data.push_back(self->CreateIRCLine(e.data));
			self->bot.BotEvents().Raise(to_net);
		}
		catch (exception& err)
		{
			self->bot.BotLog().GetLog(BotLogger::SYS).Put(WARNING, "IRCProcessor::IOHandler: Caught exception while processing (probably) malformed IRC_SEND");
			self->bot.BotLog().GetLog(BotLogger::SYS).Put(WARNING, string("IRCProcessor::IOHandler: ") + err.what());
		}
	}
	else if (e.name == CommonEvents::NET_RECV)
	{
		// this is like the only situation where we actually care about timestamps.  the network worked hard to make sure they were as accurate as possible
		// logging call
		self->RawLog(e);
		EventHandler::Event from_net;
		from_net.data = self->ParseIRCLine(e.data[1]);
		from_net.data[0] = e.data[0];
		from_net.timestamp = e.timestamp;
		from_net.name = CommonEvents::IRC_GENERAL;
		self->bot.BotEvents().Raise(from_net);

		self->AnalyzeAndClassify(from_net);
		if (from_net.name != CommonEvents::IRC_GENERAL);
		{
			self->bot.BotEvents().Raise(from_net);
			self->LogProcess(from_net);
		}
	}
	return;
}

void IRCProcessor::AnalyzeAndClassify(EventHandler::Event &e)
{
	try
	{
		if (StringUtil::ToUpper(e.data[2]) == "PRIVMSG")
		{
			if (IsChannel(e.data[3])) e.name = CommonEvents::IRC_CHANNEL_MESSAGE;
			else e.name = CommonEvents::IRC_PRIVATE_MESSAGE;
		}
		else if (StringUtil::ToUpper(e.data[2]) == "ACCOUNT") e.name = CommonEvents::IRC_AUTHCHANGE;
		else if (StringUtil::ToUpper(e.data[2]) == "JOIN") e.name = CommonEvents::IRC_USER_ENTER;
		else if (StringUtil::ToUpper(e.data[2]) == "PART") e.name = CommonEvents::IRC_USER_LEAVE;
		else if (StringUtil::ToUpper(e.data[2]) == "NICK") e.name = CommonEvents::IRC_USER_NICK;
		else if (StringUtil::ToUpper(e.data[2]) == "QUIT") e.name = CommonEvents::IRC_USER_QUIT;
		else if (StringUtil::ToUpper(e.data[2]) == "KICK") e.name = CommonEvents::IRC_USER_KICK;
		else if (StringUtil::ToUpper(e.data[2]) == "NOTICE")
		{
			if (IsChannel(e.data[3])) e.name = CommonEvents::IRC_CHANNEL_NOTICE;
			else e.name = CommonEvents::IRC_PRIVATE_NOTICE;
		}
		else if (StringUtil::ToUpper(e.data[2]) == "001") e.name = CommonEvents::IRC_WELCOME;
		else e.name = CommonEvents::IRC_OTHER;
	}
	catch (exception& e)
	{
	}
	return;
}

vector<string> IRCProcessor::ParseIRCLine(string line)
{
	vector<string> out(1);

	string segment;
	int start_pos = 0;
	int break_pos = line.find(' ');

	if (break_pos == -1) segment = line;
	else segment = line.substr(start_pos, break_pos - start_pos);
	if (segment.find(':') == 0) segment = segment.substr(1);

	while (true)
	{
		out.push_back(segment);

		start_pos = (break_pos == -1) ? -1 : line.find_first_not_of(' ', break_pos);
		break_pos = (start_pos == -1) ? -1 : line.find(' ', start_pos);

		if (start_pos == -1) break;

		if (line[start_pos] == ':')
		{
			segment = line.substr(start_pos + 1);
			break_pos = -1;
		}
		else segment = line.substr(start_pos, break_pos - start_pos);
	}

	return out;
}

string IRCProcessor::CreateIRCLine(vector<string>& components)
{
	if (components.size() < 2) return "";
	else if (components.size() == 2) return (components[1] + "\r\n");
	else
	{
		string line;

		for (int i = 1; i < components.size() - 1; ++i)
		{
			line += components[i];
			line += " ";
		}

		line += ":";
		line += components[components.size() - 1];
		line += "\r\n";
		return line;
	}
	bot.BotLog().GetLog(BotLogger::DBG).Put(CRITICAL, "NetManager::CreateIRCLine: what the fucking fuck.");
	return "";
}

void IRCProcessor::SubscribeToEvents()
{
	set<string> myevents;
	myevents.insert(CommonEvents::IRC_SEND);
	myevents.insert(CommonEvents::NET_RECV);
	bot.BotEvents().Subscribe(myevents, IOHandler, this);
	myevents.clear();
	myevents.insert(CommonEvents::NET_STARTED);
	myevents.insert(CommonEvents::IRC_WELCOME);
	bot.BotEvents().Subscribe(myevents, WelcomeHandler, this);
}

// HAAAAAAAAACKS!!!!!!!!!!1!@#!$!#%@
bool IRCProcessor::IsChannel(string s)
{
	return (s.find('#') == 0);
}

string IRCProcessor::GetLogMessage(EventHandler::Event& e)
{
	if (e.name == CommonEvents::IRC_AUTHCHANGE)
		return "== " + IRCSender(e.data[1]).nickname + ((e.data[4] == "*") ? " is no longer logged in." : " is now logged in as " + e.data[3] + ".");
	else if (e.name == CommonEvents::IRC_USER_ENTER)
		return "== " + e.data[1] + " has joined " + e.data[3] + ".";
	else if (e.name == CommonEvents::IRC_USER_LEAVE)
		return "== " + IRCSender(e.data[1]).nickname + " has left " + e.data[3] + (e.data.size() > 4 ? ". (" + e.data[4] + ")" : ".");
	else if (e.name == CommonEvents::IRC_USER_KICK)
		return "== " + IRCSender(e.data[1]).nickname + " has kicked " + e.data[4] + " from " + e.data[3] + (e.data.size() > 5 ? ". (" + e.data[5] + ")" : ".");
	else if (e.name == CommonEvents::IRC_USER_QUIT)
		return "== " + e.data[1] + " has disconnected. (" + e.data[3] + ")";
	else if (e.name == CommonEvents::IRC_USER_NICK)
		return "== " + IRCSender(e.data[1]).nickname + " is now known as " + e.data[3] + ".";
	else if (e.name == CommonEvents::IRC_CHANNEL_MESSAGE || e.name == CommonEvents::IRC_PRIVATE_MESSAGE)
		return "<" + IRCSender(e.data[1]).nickname + "> " + e.data[4];
	else if (e.name == CommonEvents::IRC_CHANNEL_NOTICE || e.name == CommonEvents::IRC_PRIVATE_NOTICE)
		return "-" + IRCSender(e.data[1]).nickname + "- " + e.data[4];
	return "";
}

void IRCProcessor::LogProcess(EventHandler::Event& e)
{
	string logname = bot.BotNet().GetConnectionName(e.data[0]) + "/" + (IsChannel(e.data[2]) ? e.data[2] : "ME");

	string message = GetLogMessage(e);
	if (message == "") return;

	if (e.name == CommonEvents::IRC_CHANNEL_MESSAGE || e.name == CommonEvents::IRC_PRIVATE_MESSAGE)
		bot.BotLog().GetLog(logname).Put(IRC_MESSAGE, message);
	else if (e.name == CommonEvents::IRC_CHANNEL_NOTICE || e.name == CommonEvents::IRC_PRIVATE_NOTICE)
		bot.BotLog().GetLog(logname).Put(IRC_NOTICE, message);
	else
		bot.BotLog().GetLog(logname).Put(IRC_OTHER, message);
	return;
}

void IRCProcessor::RawLog(EventHandler::Event& e)
{
	bot.BotLog().GetLog(BotLogger::RAW).Put(IRC_OTHER, StringUtil::StripWS(CreateIRCLine(e.data)));
}
