#include "cmd.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "UserManager.h"

cmd::cmd()
{
	acceptsflags = false;
}

void cmd::Register(IRCBot& bot)
{
	vector<string> command = CommandStrings();
	for (vector<string>::iterator i = command.begin(); i != command.end(); i++)
		bot.AddCommand((*i), this, defaultuserlevel, defaultuserflags);
	PostInstall(bot);
}

void cmd::PostInstall(IRCBot& bot)
{
}

string cmd::HelpMsg(string command)
{
	return "";
}

bool cmd::IsAlias()
{
	return false;
}

bool cmd::AcceptsFlags()
{
	return acceptsflags;
}

void cmd::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
}


void cmd::FlagTestAndGo(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args, string channel)
{
	string flags = bot.usermgr.UserFlagString(channel, speaker.GetAccountL());
	string reqflags = bot.permission.GetCommandFlags(command);
	for (int i = 0; i < flags.length(); i++)
	{
		if (reqflags.find(flags.at(i)) != reqflags.npos)
		{
			ProcessCommand(bot, command, speaker, target, respondto, args);
			break;
		}
	}
	return;
}

void cmd::SaveState()
{}
