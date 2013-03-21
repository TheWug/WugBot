#include "CmdSave.h"
#include "IRCBot.h"
#include "UserManager.h"
#include "BotPermissions.h"
#include "misc.h"
#include "module.h"
#include "ModuleHandler.h"

CmdSave::CmdSave()
{
	defaultuserlevel = 999;
}

void CmdSave::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	if (bot.readonly)
	{
		bot.Say(respondto, "Unable to save: bot is in read only mode.  Use \"option readonly off\" to enable saving.");
		return;
	}
	bot.permission.SaveState();
	bot.usermgr.SaveState();
	for (map<string, cmd *>::iterator i = bot.commands.begin(); i != bot.commands.end(); i++)
	{
		if (i->second != NULL)
			i->second->SaveState();
	}
	for (map<string, module *>::iterator i = bot.modhandler.modules.begin(); i != bot.modhandler.modules.end(); i++)
	{
		if (i->second != NULL)
			i->second->SaveState();
	}
}

string CmdSave::HelpMsg(string command)
{
	return string("Usage: ") + command + " -- saves state for all of the bots modules.  State is saved automatically upon quit, but not if the bot is terminated in an abnormal way.";
}

vector<string> CmdSave::CommandStrings()
{
	vector<string> types;
	types.push_back("save");
	return types;
}

