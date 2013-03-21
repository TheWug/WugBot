#include <map>
#include <set>

#include "IRCBot.h"
#include "alias.h"
#include "misc.h"

alias::alias(string c)
{
	command = c;
	defaultuserflags = "";
	defaultuserlevel = -1;
	helpmsg = "You can define this message with the following command: alias sethelp " + command + " <help message goes here>";
	channelarg = -1;
	acceptsflags = false;
}

bool alias::IsAlias()
{
	return true;
}

void alias::SetHelpMsg(string message)
{
	helpmsg = message;
}

void alias::AddCommand(string c)
{
	commands.push_back(c);
}

void alias::SetChannelArgument(int i)
{
	channelarg = i;
	acceptsflags = (i != -1);
}

void alias::Clear()
{
	commands.clear();
}

vector<string> alias::CommandStrings()
{
	vector<string> types;
	types.push_back(command);
	return types;
}

void alias::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	for (vector<string>::iterator c = commands.begin(); c != commands.end(); c++) // trollface
	{
		int i = 0, j = 0;
		string commandstring = *c;
		commandstring = stringreplace(
				stringreplace(
				stringreplace(
				stringreplace(
				stringreplace(
				stringreplace(	commandstring, "%%", "\t")
						, "%n", speaker.GetNick())
						, "%h", speaker.GetHost())
						, "%t", target)
						, "%r", respondto)
						, "\t", "%");
		ReplaceTokensWithArgs(commandstring, args);
		bot.PerformCommand(speaker, target, respondto, commandstring);
	}
}

void alias::ProcessCommandFlags(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	vector<string> arguments = splitcommands(args, " ", false);
	if (arguments.size() <= channelarg)
		return;
	FlagTestAndGo(bot, command, speaker, target, respondto, args, tolower(arguments[channelarg]));
}

string alias::HelpMsg(string command)
{
	return helpmsg;
}

void alias::Unregister(IRCBot& bot)
{
	bot.RemoveCommand(command);
}

void alias::ReplaceTokensWithArgs(string& haystack, string argstring)
{
	static const string pct = "%", amp = "&";
	vector<string> arguments = splitcommands(argstring, " ", false);
	argstring = "";
	if (arguments.size() != 0)
		argstring = arguments[0];
	for (int i = 1; i < arguments.size(); i++)
		argstring += " " + arguments[i];
	for (int i = haystack.find('%'); i != -1; i = haystack.find('%', i + 1))
	{
		int value = -1;
		istringstream s(haystack.substr(i + 1));
		s >> value;
		if (!s)
			continue;
		string numbers = pct + tostr(value);
		if (value < arguments.size())
			haystack.replace(i, numbers.length(), arguments[value]);
		else
			haystack.replace(i, numbers.length(), "");
	}
	for (int i = haystack.find('&'); i != -1; i = haystack.find('&', i + 1))
	{
		int value = -1;
		istringstream s(haystack.substr(i + 1));
		s >> value;
		if (!s)
			continue;
		string numbers = pct + tostr(value);
		if (value < arguments.size())
		{
			int x = 0;
			for (int j = 0; j < value; j++)
				x += arguments[j].length() + 1;
			haystack.replace(i, numbers.length(), argstring.substr(x));
		}
		else
			haystack.replace(i, numbers.length(), "");
	}
}
