#include <sstream>

#include "CmdMemo.h"
#include "IRCBot.h"
#include "misc.h"
#include "DeferredCommands/MemoSender.h"

CmdMemo::CmdMemo(IRCBot& bot) : memosender(*(new MemoSender(bot)))
{
	defaultuserlevel = 0;
	bot.AddDeferredCommand(&memosender);
}

CmdMemo::~CmdMemo()
{
	delete &memosender;
}

void CmdMemo::ProcessCommand(IRCBot& bot, string c, Hostname speaker, string target, string respondto, string args)
{
	istringstream t(args);
	string user = "", command = "";
	t >> command;
	if (tolower(command) == "send")
	{
		t >> user;
		if (!IsValidNick(user))
			bot.Say(respondto, user + " is not a valid nickname.");
		else
		{
			if (memosender.AddMemo(MemoSender::GetSenderString(speaker), MemoSender::GetTargetString(user), getline(t)))
				bot.Say(respondto, "I'll pass that on to " + user + " when next he's active.");
			else
				bot.Say(respondto, user + " has opted out of receiving memos, you will need to contact them yourself.");
		}
	}
	else if (tolower(command) == "view-unsent")
	{
		t >> user;
		string memos = memosender.ViewSent(MemoSender::GetSenderString(speaker), MemoSender::GetTargetString(user));
		if (memos == "")
			bot.Say(respondto, "You have no unsent memos.");
		else
			bot.Say(speaker.GetNick(), memos);
	}
	else if (tolower(command) == "delete")
	{
		int number = -1;
		t >> number;
		if (number < 0)
			bot.Say(respondto, "You must enter a valid number.  use memo list-sent to see the numbers of sent memos.");
		else
		{
			if (memosender.DeleteSent(MemoSender::GetSenderString(speaker), number))
				bot.Say(respondto, "Memo deleted.");
			else
				bot.Say(respondto, "Nonexistant memo.  Perhaps it has been sent?");
		}
	}
	else if (tolower(command) == "delete-to")
	{
		t >> user;
		if (user == "")
			bot.Say(respondto, "You must specify a user.");
		else
		{
			memosender.DeleteTo(MemoSender::GetSenderString(speaker), MemoSender::GetTargetString(user));
			bot.Say(respondto, "Deleted all unsent memos from you to " + user + ".");
		}
	}
	else if (tolower(command) == "block")
	{
		if (speaker.GetAccountL() == Hostname::unauthenticated || speaker.GetAccountL() == Hostname::undetermined)
			bot.Say(respondto, "Only authenticated users may opt out of receiving memos.");
		else if (!memosender.CanSend(MemoSender::GetSenderString(speaker)))
			bot.Say(respondto, "You have already opted out of receiving memos.");
		else
		{
			memosender.Block(MemoSender::GetSenderString(speaker));
			bot.Say(respondto, "The account " + speaker.GetAccount() + " has opted out of receiving memos.");
		}
	}
	else if (tolower(command) == "unblock")
	{
		if (speaker.GetAccountL() == Hostname::unauthenticated || speaker.GetAccountL() == Hostname::undetermined)
			bot.Say(respondto, "Only authenticated users may opt into or out of receiving memos.");
		else if (memosender.CanSend(MemoSender::GetSenderString(speaker)))
			bot.Say(respondto, "You have already opted into receiving memos.");
		else
		{
			memosender.Unblock(MemoSender::GetSenderString(speaker));
			bot.Say(respondto, "The account " + speaker.GetAccount() + " has opted into receiving memos.");
		}
	}
	else
		bot.Say(respondto, "Unrecognized subcommand.");
}

string CmdMemo::HelpMsg(string command)
{
	return "Usage: memo <subcommand> <arguments> -- Sends a memo to a user, or controls memos you have sent.  Subcommand can be one of the following: send, view-unsent, delete, delete-to, block, unblock.\nSend takes a user and a memo, view-sent takes an optional user, delete takes a memo index, delete-to takes a user, and block and unblock take no arguments.";
}

vector<string> CmdMemo::CommandStrings()
{
	vector<string> types;
	types.push_back("memo");
	return types;
}
