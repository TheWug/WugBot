#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "CmdQueue.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"

CmdQueue::CmdQueue()
{
	functions["add"] = &CmdQueue::Add;
	functions["remove"] = &CmdQueue::Remove;
	functions["pop"] = &CmdQueue::Pop;
	functions["bump"] = &CmdQueue::Bump;
	functions["sink"] = &CmdQueue::Sink;
	functions["invite"] = &CmdQueue::Invite;
	functions["kick"] = &CmdQueue::Kick;
	functions["where"] = &CmdQueue::Where;
	functions["list"] = &CmdQueue::List;
	levelreqs["add"] = 0;
	levelreqs["remove"] = 0;
	levelreqs["pop"] = 0;
	levelreqs["bump"] = 0;
	levelreqs["sink"] = 0;
	levelreqs["invite"] = 0;
	levelreqs["kick"] = 0;
	levelreqs["where"] = 0;
	levelreqs["list"] = 0;
	defaultuserlevel = 0;
}

vector<string> CmdQueue::CommandStrings()
{
	vector<string> types;
	types.push_back("queue");
	types.push_back("allow-queue-subcommand");
	types.push_back("list");
	return types;
}

void CmdQueue::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	if (command == "allow-queue-subcommand")
	{
		vector<string> arguments = splitcommands(args, " ", false);
		if (arguments.size() < 1)
		{
			bot.Say(respondto, "You must specify a subcommand.");
			return;
		}
		string action = arguments[0];
		if (levelreqs.find(action) == levelreqs.end())
		{
			bot.Say(respondto, "Invalid subcommand.");
			return;
		}
		if (arguments.size() < 2)
		{
			bot.Say(respondto, "Userlevel requirement for \"queue " + action + "\" is " + tostr(levelreqs[action]) + ".");
			return;
		}
		int inum = -2;
		istringstream(arguments[1]) >> inum;
		if (inum < -1)
			bot.Say(respondto, "Invalid userlevel specified.");
		else
		{
			bot.Say(respondto, "Userlevel requirement for \"queue " + action + "\" set to " + tostr(inum) + ".");
			levelreqs[action] = inum;
		}
		return;
	}
	string item = speaker.GetNick();
	string queuename = target;
	string action = "add";
	if (queuename == bot.nick)
		queuename = "#";
	istringstream stream(args);
	string temp;
	stream >> temp;
	if (temp.length() > 0 && temp.at(0) == '#')
	{
		queuename = temp;
		stream >> temp;
	}
	if (temp.length() > 0)
		action = temp;
	temp = getline(stream);
	if (temp.length() > 0)
		item = temp;
	queuename = tolower(queuename);
	action = tolower(action);
	map<string, queuefunction>::iterator f = functions.find(action);
	if (f != functions.end())
	{
		if (levelreqs[f->first] > bot.permission.GetUserLevel(speaker.GetAccountL()) && speaker.GetAccountL() != bot.permission.GetOwner())
			bot.Say(respondto, "You are not allowed to use that subcommand.");
		else
			(this->*(f->second))(bot, item, queuename, speaker, target, respondto, true);
	}
	else
		bot.Say(respondto, "Invalid operation: '" + action + "' is not a valid action");
	return;
}

void CmdQueue::Add(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak)
{
	queues[queuename].push_back(item);
	bot.Say(respondto, "'" + item + "' has been added to '" + queuename + "'.");
}

void CmdQueue::Remove(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak)
{
	list<string>::iterator found = queues[queuename].begin();
	for (; found != queues[queuename].end() && (*found) != item; found++);
	if (found == queues[queuename].end())
	{
		bot.Say(respondto, "'" + item + "' is not in the list.");
		return;
	}
	queues[queuename].erase(found);
	if (queues[queuename].size() == 0)
		queues.erase(queuename);
	bot.Say(respondto, "'" + item + "' has been removed from '" + queuename + "'.");
}

void CmdQueue::Pop(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak)
{
	if (queues.find(queuename) == queues.end())
	{
		bot.Say(respondto, "Queue '" + queuename + "' is empty.");
		return;
	}
	item = queues[queuename].front();
	int remaining = queues[queuename].size() - 1;
	if (queues[queuename].size() == 1)
		queues.erase(queuename);
	else
		queues[queuename].erase(queues[queuename].begin());
	bot.Say(respondto, "'" + item + "' has been removed from '" + queuename + "'.  " + (remaining == 0 ? "The queue is now empty." : "There are " + tostr(remaining) + " remaining people in the queue."));
}

void CmdQueue::Bump(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak)
{
	list<string>::iterator found = queues[queuename].begin();
	for (; found != queues[queuename].end() && (*found) != item; found++);
	if (found == queues[queuename].end())
	{
		bot.Say(respondto, "'" + item + "' is not in the list.");
		return;
	}
	string temp = (*found);
	queues[queuename].erase(found);
	queues[queuename].push_front(temp);
	bot.Say(respondto, "'" + item + "' has been moved to the front of the queue.");
}

void CmdQueue::Sink(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak)
{
	list<string>::iterator found = queues[queuename].begin();
	for (; found != queues[queuename].end() && (*found) != item; found++);
	if (found == queues[queuename].end())
	{
		bot.Say(respondto, "'" + item + "' is not in the list.");
		return;
	}
	string temp = (*found);
	queues[queuename].erase(found);
	queues[queuename].push_back(temp);
	bot.Say(respondto, "'" + item + "' has been moved to the back of the queue.");
}

void CmdQueue::Invite(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak)
{
	if (queues[queuename].size() == 0)
	{
		bot.Say(respondto, "The queue is empty.");
		return;
	}
	string user = queues[queuename].front();
	string channel = item;
	if (item.length() == 0 || item.at(0) != '#')
		channel = target;
	if (channel.at(0) != '#')
		bot.Say(respondto, "I can only invite users to channels. Please use this command in a channel, or specify a channel as the final argument.");
	else
	{
		bot.Raw("INVITE " + user + " " + channel);
		Pop(bot, "", queuename, speaker, target, respondto);
	}
}

void CmdQueue::Kick(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak)
{
	if (queues[queuename].size() == 0)
	{
		bot.Say(respondto, "The queue is empty.");
		return;
	}
	string user = queues[queuename].front();
	string channel = item;
	if (item.length() == 0 || item.at(0) != '#')
		channel = target;
	if (channel.at(0) != '#')
		bot.Say(respondto, "I can only kick users from channels. Please use this command in a channel, or specify a channel as the final argument.");
	else
	{
		bot.Kick(user, channel, speaker.GetNick() + " has removed you from the queue.");
		Pop(bot, "", queuename, speaker, target, respondto);
	}
}

void CmdQueue::Where(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak)
{
	list<string>::iterator found = queues[queuename].begin();
	int i = 1;
	for (; found != queues[queuename].end() && (*found) != item; i++, found++);
	if (found == queues[queuename].end())
	{
		bot.Say(respondto, "'" + item + "' is not in the list.");
		return;
	}
	bot.Say(respondto, "'" + item + "' is at position " + tostr(i) + ".");
}

void CmdQueue::List(IRCBot& bot, string item, string queuename, Hostname speaker, string target, string respondto, bool speak)
{
	list<string>::iterator found = queues[queuename].begin();
	int i = 1;
	string currentqueue = "Queue '" + queuename + "':\n";
	for (; found != queues[queuename].end(); i++)
	{
		currentqueue.append(tostr(i) + ": " + (*found));
		if (++found != queues[queuename].end())
			currentqueue.append(" | ");
	}
	if (i == 1)
	{
		bot.Say(respondto, "'" + queuename + "' is empty.");
		return;
	}
	bot.Say(respondto, currentqueue);
}

string CmdQueue::HelpMsg(string command)
{
	if (command == "allow-queue-subcommand")
		return "Usage: allow-queue-subcommand <subcommand> [userlevel] -- changes the userlevel requirement for subcommands of queue.  use 'help queue' for the list of available subcommands.  If no userlevel is specified, the current requirement will be displayed.";
	return "Usage: queue [name] [action] [item] -- performs specified action with provided named queue.  If name is omitted, current channel name is used.  If user is omitted, your own nick is used.  If action is omitted, 'add' is implied.  If you specify a parameter, you must specify the preceding ones (except for name).\nAvailable subcommands: add remove pop bump sink invite kick where list";
}

void CmdQueue::PostInstall(IRCBot& bot)
{
	ifstream pfile("IRCBot.queues");
	int maxsize = 512;
	int level = bot.permission.GetCommandLevel("queue");
	for (map<string, int>::iterator i = levelreqs.begin(); i != levelreqs.end(); i++)
		(*i).second = level;
	string q = "allow-queue-subcommand";
	bot.permission.SetDefaultCommandLevel(q, 999);
	while (true)
	{
		string queuename;
		char queueitem[maxsize];
		int queuesize;
		pfile >> queuename;
		pfile >> queuesize;
		pfile.getline(queueitem, maxsize);
		if (!pfile)
			break;
		for (int i = 0; i < queuesize; i++)
		{
			pfile.getline(queueitem, maxsize);
			queues[queuename].push_back(queueitem);
		}
	}
	pfile.close();
}

void CmdQueue::SaveState()
{
	ofstream pfile("IRCBot.queues");
	for (map<string, list<string> >::iterator i = queues.begin(); i != queues.end(); i++)
	{
		pfile << i->first << " " << i->second.size() << endl;
		for (list<string>::iterator j = i->second.begin(); j != i->second.end(); j++)
			pfile << *j << endl;
	}
	pfile.close();
}
