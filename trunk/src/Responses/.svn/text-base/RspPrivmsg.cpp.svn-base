#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include "RspPrivmsg.h"

#include "IRCBot.h"
#include "BotPermissions.h"
#include "misc.h"

using namespace std;

void RspPrivmsg::PreAction(IRCBot& bot, Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	if (ctcpflag)
	{
		istringstream resp(message);
		string ctcpcommand;
		resp >> ctcpcommand;
		string ctcpmessage = getline(resp);
		ctcpcommand = toupper(ctcpcommand);
		if (ctcpcommand == "ACTION")
			cout << setw(32) << left << "~" + target + "/" + speaker.GetNick() + "~" << "* " + speaker.GetNick() + " " + ctcpmessage << endl;
		else
			cout << setw(32) << left << "<" + speaker.GetNick() + ">" << "Received CTCP " + ctcpcommand << endl;
		bot.ProcessCTCP(speaker, command, target, ctcpcommand, ctcpmessage);
	}
	else
	{
		string messagelowercase = tolower(message);
		cout << setw(32) << left << "~" + target + "/" + speaker.GetNick() + "~" << " " + message << endl;
		bool hascommand = false;
		string respondto = bot.GetRespondTo(speaker, target);

		if (bot.permission.CanBeUsedBy(speaker))
		{
			if (messagelowercase.find(bot.lnick) == 0 && messagelowercase.length() > bot.lnick.length() && !IsAlphaNumeric(messagelowercase.at(bot.lnick.length())))
			{
				istringstream temp(message);
				string derp; temp >> derp;
				if (target.at(0) == '#')
					hascommand = bot.ParseCommand(speaker, target, respondto, getline(temp));
				else
					hascommand = bot.ParseCommand(speaker, target, respondto, getline(temp));
			}
			else if (messagelowercase.find(bot.commandprefix) == 0)
			{
				string temp(message.c_str() + 1);
				if (target.at(0) == '#')
					hascommand = bot.ParseCommand(speaker, target, respondto, temp);
				else
					hascommand = bot.ParseCommand(speaker, target, respondto, temp);
			}
			else if (target.at(0) != '#')
				hascommand = bot.ParseCommand(speaker, target, respondto, message);
		}

		if (!hascommand && bot.permission.CanBeUsedBy(speaker) && bot.permission.CanSendTo(target))
			bot.ChatResponse(speaker, target, respondto, message);
	}
	return;
}

string RspPrivmsg::RespondTo()
{
	return "privmsg";
}
