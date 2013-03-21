#include <iostream>
#include <stdlib.h>

#include "CmdFwd.h"
#include "Forwarder.h"
#include "IRCBot.h"
#include "misc.h"

CmdFwd::CmdFwd()
{
	defaultuserlevel = 3;
}

vector<string> CmdFwd::CommandStrings()
{
	vector<string> types;
	types.push_back("fwd");
	types.push_back("forward");
	return types;
}

void CmdFwd::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream gc(args);
	string subc;
	gc >> subc;
	if (subc == "add" || subc == "+")
		Add(bot, speaker, target, respondto, getline(gc));
	else if (subc == "remove" || subc == "-")
		Remove(bot, speaker, target, respondto, getline(gc));
	else if (subc == "removeall")
		RemoveAll(bot, speaker, target, respondto, getline(gc));
	else
		bot.Say(respondto, "Invalid subcommand.");
}

string CmdFwd::HelpMsg(string command)
{
	return "Usage: " + command + " <+|add|-|remove|removeall> <source> <target> -- forwards messages sent to the provided source to the provided target.  Source and target can be channels or users, but if the source is a user, no user other than the bots name will have any effect.";
}

void CmdFwd::Add(IRCBot& bot, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	if (arguments.size() == 0)
		bot.Say(respondto, "Forward what to what?");
	else if (arguments.size() == 1)
		bot.Say(respondto, "I can't forward something to nothing.");
	else
	{
		string nsubj = tolower(arguments[0]), ntar = tolower(arguments[1]);
		for (vector<dcmd *>::iterator i = forwardlist.begin(); i != forwardlist.end(); i++)
		{
			Forwarder * f = (Forwarder *) (*i);
			if (f->GetSubject() == nsubj && f->GetTarget() == ntar)
			{
				bot.Say(respondto, "I'm already forwarding that to that.");
				return;
			}
		}
		Forwarder * f = new Forwarder(nsubj, ntar, bot);
		bot.AddDeferredCommand(f);
		forwardlist.push_back(f);
		bot.Say(respondto, "Now forwarding messages from " + arguments[0] + " to " + arguments[1] + ".");
	}
	return;
}

void CmdFwd::Remove(IRCBot& bot, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	if (arguments.size() == 0)
		bot.Say(respondto, "Stop forwarding what to what?");
	else if (arguments.size() == 1)
		bot.Say(respondto, "I can't stop forwarding something to nothing.");
	else
	{
		string nsubj = tolower(arguments[0]), ntar = tolower(arguments[1]);
		for (vector<dcmd *>::iterator i = forwardlist.begin(); i != forwardlist.end(); i++)
		{
			Forwarder * f = (Forwarder *) (*i);
			if (f->GetSubject() == nsubj && f->GetTarget() == ntar)
			{
				bot.RemoveDeferredCommand(f);
				forwardlist.erase(i);
				bot.Say(respondto, "No longer forwarding messages from " + arguments[0] + " to " + arguments[1] + ".");
				return;
			}
		}
		bot.Say(respondto, "I'm not forwarding " + arguments[0] + " to " + arguments[1] + ".");
	}
	return;
}

void CmdFwd::RemoveAll(IRCBot& bot, Hostname speaker, string target, string respondto, string args)
{
	int c = 0;
	for (vector<dcmd *>::iterator i = forwardlist.begin(); i != forwardlist.end(); i++)
	{
		dcmd * f = (*i);
		bot.RemoveDeferredCommand(f);
		c++; // :)
	}
	forwardlist.clear();
	ostringstream message;
	message << "Stopped " << c << " active forwards.";
	bot.Say(respondto, message.str());
	return;
}
