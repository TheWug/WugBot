#include <iostream>
#include <stdlib.h>

#include "CmdFactoid.h"
#include "FactoidProcessor.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"

CmdFactoid::CmdFactoid(IRCBot& b) : bot(b)
{
	factoidprocessor = new FactoidProcessor(b);
	bot.AddDeferredCommand(factoidprocessor);
	defaultuserlevel = 0;
}

vector<string> CmdFactoid::CommandStrings()
{
	vector<string> types;
	types.push_back("factoid");
	types.push_back("lock");
	types.push_back("unlock");
	types.push_back("+");
	types.push_back("++");
	types.push_back("~");
	types.push_back("-");
	types.push_back("search");
	return types;
}

void CmdFactoid::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	istringstream itemtext(args);
	string itemname;
	itemtext >> itemname;
	if (command == "+")
	{
		if (factoidprocessor->AddFactoid(itemname, getline(itemtext), speaker, target, respondto) == FactoidProcessor::OK)
			bot.Notice(speaker.GetNick(), "Done.");
		else
			bot.Notice(speaker.GetNick(), "That factoid already exists.");
	}
	else if (command == "++")
	{
		if (factoidprocessor->AppendFactoid(itemname, getline(itemtext), speaker, target, respondto) == FactoidProcessor::OK)
			bot.Notice(speaker.GetNick(), "Done.");
		else
			bot.Notice(speaker.GetNick(), "That factoid does not exist.");
	}
	else if (command == "lock")
	{
		int s = factoidprocessor->Lock(itemname, bot.permission.GetUserLevel(speaker.GetAccountL()) + (speaker.GetNickL() == tolower(itemname) ? 1 : 0));
		if (s == FactoidProcessor::OK)
			bot.Notice(speaker.GetNick(), "Done.");
		else if (s == FactoidProcessor::LOCKED)
			bot.Say(respondto, "You cannot lock that factoid.");
		else if (s == FactoidProcessor::BADSTATE)
			bot.Say(respondto, "That factoid is already locked.");
	}
	else if (command == "unlock")
	{
		int s = factoidprocessor->Unlock(itemname, bot.permission.GetUserLevel(speaker.GetAccountL()) + (speaker.GetNickL() == tolower(itemname) ? 1 : 0));
		if (s == FactoidProcessor::OK)
			bot.Notice(speaker.GetNick(), "Done.");
		else if (s == FactoidProcessor::LOCKED)
			bot.Say(respondto, "You cannot unlock that factoid.");
		else if (s == FactoidProcessor::BADSTATE)
			bot.Say(respondto, "That factoid is already unlocked.");
	}
	else if (command == "~")
	{
		int result = factoidprocessor->ModifyFactoid(itemname, getline(itemtext), speaker, target, respondto);
		if (result == FactoidProcessor::OK)
			bot.Notice(speaker.GetNick(), "Done.");
		else if (result == FactoidProcessor::BADSTATE)
			bot.Notice(speaker.GetNick(), "That factoid doesn't exist.");
		else if (result == FactoidProcessor::LOCKED)
			bot.Notice(speaker.GetNick(), "That factoid is locked, and you cannot change it.");
	}
	else if (command == "-")
	{
		int result = factoidprocessor->RemoveFactoid(itemname, speaker, target, respondto);
		if (result == FactoidProcessor::OK)
			bot.Notice(speaker.GetNick(), "Done.");
		else if (result == FactoidProcessor::BADSTATE)
			bot.Notice(speaker.GetNick(), "That factoid doesn't exist.");
		else if (result == FactoidProcessor::LOCKED)
			bot.Notice(speaker.GetNick(), "That factoid is locked, and you cannot change it.");
	}
	else if (command == "search")
	{
		vector<string> searchresult;
		factoidprocessor->Search(searchresult, args);
		if (searchresult.size() == 0)
			bot.Say(respondto, "No matches for \"" + args + "\".");
		else
		{
			string results = "";
			for (int i = 0; i < searchresult.size(); ++i) results += searchresult[i] + " ";
			bot.Say(respondto, "Matches for \"" + args + "\": " + results);
		}
	}
	else if (command == "factoid")
	{
		if (itemname == "disable")
		{
			string location = target;
			itemtext >> location;
			factoidprocessor->Disable(tolower(location));
			bot.Say(respondto, "Factoids will no longer be sent to " + location + ".");
		}
		if (itemname == "enable")
		{
			string location = target;
			itemtext >> location;
			factoidprocessor->Enable(tolower(location));
			bot.Say(respondto, "Factoids will be sent to " + location + " again.");
		}
		if (itemname == "ban")
		{
			string person;
			itemtext >> person;
			if (person == "")
				bot.Say(respondto, "You must specify an account.");
			else
			{
				factoidprocessor->Ban(person);
				bot.Say(respondto, person + " is no longer allowed to use factoids.");
			}
		}
		if (itemname == "unban")
		{
			string person;
			itemtext >> person;
			if (person == "")
				bot.Say(respondto, "You must specify an account.");
			else
			{
				factoidprocessor->Unban(person);
				bot.Say(respondto, person + " can use factoids again.");
			}
		}
		if (itemname == "linelimit")
		{
			int newlimit;
			itemtext >> newlimit;
			if (!itemtext)
				bot.Say(respondto, "Factoid line limit is currently: " + tostr(factoidprocessor->linelimit));
			else
			{
				factoidprocessor->linelimit = newlimit;
				bot.Say(respondto, "Factoid line limit set to: " + tostr(newlimit));
			}
		}
	}
}

void CmdFactoid::PostInstall(IRCBot& bot)
{
	string s = "factoid";
	bot.permission.SetDefaultCommandLevel(s, 2);
	factoidprocessor->LoadFactoidDB();
}

string CmdFactoid::HelpMsg(string command)
{
	if (command == "factoid")
		return "Factoids: factoids are used by saying a colon, and then the name of the factoid.  (Example: \":factoid\")\nTo assign a factoid, use ;+ <factoid> <description>.  To change a factoid, use ;~ <factoid> <description>.  To remove a factoid, use ;- <factoid>.  To append to an existing factoid, use ;++ <factoid> <more info>.";
	if (command == "+")
		return "Usage: + <factoid> <description> -- Creates a new factoid.";
	if (command == "-")
		return "Usage: - <factoid> -- Deletes an existing factoid.";
	if (command == "~")
		return "Usage: ~ <factoid> <description> -- Modifies an existing factoid.";
	if (command == "++")
		return "Usage: ++ <factoid> <more description> -- Appends to an existing factoid.";
	if (command == "lock")
		return "Usage: lock <factoid> -- Locks a factoid.  When locked, a factoid cannot be changed.  Once locked, only someone with the locker's userlevel or higher can unlock the factoid.  Users with level 0 cannot lock factoids.";
	if (command == "unlock")
		return "Usage: unlock <factoid> -- Unlocks a factoid.  Users can only unlock factoids that were locked by someone with a userlevel equal to or lower than theirs.  Users with level 0 cannot unlock factoids.";
	if (command == "search")
		return "Usage: search <query> -- Searches the database of factoids for one containing the specified string.  Searches both factoid names and contents.";
}

void CmdFactoid::SaveState()
{
	factoidprocessor->SaveFactoidDB();
}
