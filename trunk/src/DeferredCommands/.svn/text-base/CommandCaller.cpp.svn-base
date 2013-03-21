#include <string>

#include "CommandCaller.h"
#include "IRCBot.h"
#include "BotPermissions.h"
#include "cmd.h"

using namespace std;

CommandCaller::CommandCaller(IRCBot& bot, string newcommand, string newarguments) : dcmd(Hostname(""), "", bot)
{
	command = newcommand;
	arguments = newarguments;
}

bool CommandCaller::Call(string c, Hostname speaker, string target, string respondto, string message, bool ctcpflag)
{
	if (bot.permission.CanUseCommand(speaker.GetAccountL(), command))
	{
		map<string, cmd *>::iterator found = bot.commands.find(command);
		(found->second)->ProcessCommand(bot, command, speaker, target, respondto, arguments);
	}
	else
	{
		cout << "Warning: " + speaker.GetNick() + " tried to use privilaged command: " + command << endl;
		bot.Say(respondto, "You are not allowed to use command: " + command);
	}
	return true;
}
