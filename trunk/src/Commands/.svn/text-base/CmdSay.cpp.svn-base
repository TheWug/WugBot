#include <iostream>
#include <stdlib.h>

#include "CmdSay.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"

CmdSay::CmdSay()
{
	defaultuserlevel = 1;
}

void CmdSay::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	if (command != "watch")
	{
		if (tolower(target) == bot.lnick)
		{
			if (watchedchannels[speaker.GetNickL()] == "")
				bot.Say(respondto, "I can't. I'm not watching a channel.");
			else if (!bot.permission.CanSendTo(watchedchannels[speaker.GetNickL()]))
				bot.Say(respondto, "I can't speak to that channel.");
			else
				Say(bot, command, watchedchannels[speaker.GetNickL()], args);
		}
		else if (target.at(0) == '#')
		{
			if (!bot.permission.CanSendTo(target))
				bot.Say(respondto, "I can't speak to that channel.");
			else
				Say(bot, command, respondto, args);
		}
	}
	else
	{
		istringstream t(args);
		t >> args;
		if (args.length() == 0 || !(args.at(0) == '#' || ((args.at(0) == '@' || args.at(0) == '+') && args.at(1) == '#')))
			bot.Say(respondto, "I can only watch channels.");
		else
			watchedchannels[speaker.GetNickL()] = args;
	}
	return;
}

void CmdSay::Say(IRCBot& bot, string command, string target, string message)
{
	if (target.length() != 0 && (target.at(0) == '+' || target.at(0) == '@') && command != "note")
		target = target.substr(1);
	if (command == "say")
		bot.Say(target, message);
	else if (command == "msg" || command == "message")
		bot.Message(target, message);
	else if (command == "note")
		bot.Notice(target, message);
	else if (command == "act" || command == "do")
		bot.Action(target, message);
}

vector<string> CmdSay::CommandStrings()
{
	vector<string> types;
	types.push_back("say");
	types.push_back("msg");
	types.push_back("message");
	types.push_back("act");
	types.push_back("do");
	types.push_back("note");
	types.push_back("watch");
	return types;
}

string CmdSay::HelpMsg(string command)
{
	command = tolower(command);
	if (tolower(command) == "say")
		return "Usage: say <message> -- Makes the bot say the message.  If used in a channel, it will say it to the channel.  If used in pm, the bot must be watching a channel, and the message will be said to that channel.  See also: tell watch";
	if (tolower(command) == "message" || tolower(command) == "msg")
		return "Usage: say <message> -- Makes the bot say the message as a normal message.  If used in a channel, it will say it to the channel.  If used in pm, the bot must be watching a channel, and the message will be said to that channel.  See also: tell watch";
	if (tolower(command) == "act" || tolower(command) == "do")
		return "Usage: act <action> -- Makes the bot say the message with a CTCP action (/me).  If used in a channel, it will say it to the channel.  If used in pm, the bot must be watching a channel, and the message will be said to that channel.  See also: tell watch";
	if (tolower(command) == "note")
		return "Usage: note <message> -- Makes the bot say the message as a notice instead of a regular message.  If used in a channel, it will say it to the channel.  If used in pm, the bot must be watching a channel, and the message will be said to that channel.  See also: tell watch";
	else
		return "Usage: watch <channel> -- Makes the bot watch a channel.  Use of the say command in PM to the bot will cause it to speak to this channel.  See also: say tell";
}
