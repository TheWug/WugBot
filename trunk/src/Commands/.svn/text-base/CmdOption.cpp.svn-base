#include <sstream>

#include "CmdOption.h"

#include "IRCBot.h"
#include "Timer.h"
#include "BotPermissions.h"
#include "misc.h"
#include "types.h"
#include "RspInvite.h"

CmdOption::CmdOption()
{
	defaultuserlevel = -1;
}

void CmdOption::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	string key, value;
	{
		istringstream s(args);
		s >> key;
		value = trim(getline(s));
	}
	if ((speaker.GetAccountL() != tolower(bot.permission.GetOwner())) && (value != ""))
	{
		bot.Say(respondto, "Only the bot owner can set bot options. You are " + speaker.GetAccountL() + " and you need to be " + tolower(bot.permission.GetOwner()) + ".");
		return;
	}
	if (key == "")
	{
		bot.Say(respondto, "Available options: throttle, owner, silent, readonly, invite, logging");
		return;
	}
	else if (key == "logging")
	{
		if (value == "")
			bot.Say(respondto, "Bot log target: " + bot.logtarget);
		else
		{
			bot.Say(respondto, "Setting bot log target to " + value);
			bot.logtarget = value;
		}
	}
	else if (key == "invite")
	{
		if (value == "")
		{
			bot.Say(respondto, "Available modes are: join, notice, request, ignore");
		}
		else
		{
			value = tolower(value);
			RspInvite * invitersp = ((RspInvite *) bot.responses["invite"]);
			if (value == "join")
				invitersp->SetInviteMode(JOIN);
			else if (value == "notice")
				invitersp->SetInviteMode(NOTICE);
			else if (value == "request")
				invitersp->SetInviteMode(REQUEST);
			else if (value == "ignore")
				invitersp->SetInviteMode(IGNORE);
			else
			{
				bot.Say(respondto, "Invalid mode.  Available modes are: join, notice, request, ignore.");
				return;
			}
			bot.Say(respondto, "Set autojoin on invite mode to: " + value);
		}
	}
	else if (key == "owner")
	{
		if (value == "")
		{
			bot.Say(respondto, "Bot Owner: " + bot.permission.GetOwner());
			return;
		}
		else
		{
			bot.Say(respondto, "This option cannot be set and must be specified in the config file.");
		}
	}
	else if (key == "throttle")
	{
		if (value == "")
		{
			bot.Say(respondto, "Flood limiting settings: min,weight,max = " +
						tostr(bot.network.GetTimer().GetMinInterval()) + "," + 
						tostr(bot.network.GetTimer().GetEventWeight()) + "," + 
						tostr(bot.network.GetTimer().GetMaxAllowedWeight()));
		}
		else
		{
			vector<string> floodsettings = splitcommands(value, ",", true);
			int s = floodsettings.size();
			
			float min = bot.network.GetTimer().GetMinInterval();
			float weight = bot.network.GetTimer().GetEventWeight();
			float max = bot.network.GetTimer().GetMaxAllowedWeight();
			if (s > 0)
			{
				istringstream ss(floodsettings.at(0));
				ss >> min;
			}
			if (s > 1)
			{
				istringstream ss(floodsettings.at(1));
				ss >> weight;
			}
			if (s > 2)
			{
				istringstream ss(floodsettings.at(2));
				ss >> max;
			}
			if (min < 0 || max < 0 || weight < 0)
				bot.Say(respondto, "arguments cannot be less than 0.");
			else if (weight > max)
				bot.Say(respondto, "maximum weight cannot be less than event weight.");
			else
			{
				bot.network.GetTimer().SetMinInterval(min);
				bot.network.GetTimer().SetEventWeight(weight);
				bot.network.GetTimer().SetMaxAllowedWeight(max);
				bot.Say(respondto, "Flood limiting settings changed: min,weight,max = " + tostr(min) + "," + tostr(weight) + "," + tostr(max));
			}
		}
	}
	else if (key == "silent")
	{
		value = tolower(value);
		if (value == "")
		{
			bot.Say(respondto, "Silent mode is currently " + string(bot.silent ? "enabled." : "disabled."));
			return;
		}
		else if (value == "true" || value == "yes" || value == "on" || value == "y" || value == "t")
			bot.silent = true;
		else if (value == "false" || value == "off" || value == "no" || value == "n" || value == "f")
			bot.silent = false;
		bot.Say(respondto, "Silent mode has been " + string(bot.silent ? "enabled." : "disabled."));
	}
	else if (key == "readonly")
	{
		value = tolower(value);
		if (value == "")
		{
			bot.Say(respondto, "Read only mode is currently " + string(bot.readonly ? "enabled." : "disabled."));
			return;
		}
		else if (value == "true" || value == "yes" || value == "on" || value == "y" || value == "t")
			bot.readonly = true;
		else if (value == "false" || value == "off" || value == "no" || value == "n" || value == "f")
			bot.readonly = false;
		bot.Say(respondto, "Read only mode has been " + string(bot.readonly ? "enabled." : "disabled."));
	}
}

string CmdOption::HelpMsg(string command)
{
	return "Usage: option [key] [value] -- Sets a bot configuration option.  If no value is specified, it will print the current setting for the specified key.  If no key is specified, it will print a list of keys.";
}

vector<string> CmdOption::CommandStrings()
{
	vector<string> types;
	types.push_back("option");
	return types;
}

