#include "IRC.h"
#include "ServerProperties.h"
#include "misc.h"

bool IRC::IsValidNick(ServerProperties& p, string testnick)
{
	return true;
}

bool IRC::IsValidChannel(ServerProperties& p, string testchan)
{
	return true;
}

string IRC::Monitor(string action, string target)
{
	if (target.length() == 0)
		return "MONITOR " + action + "\r\n";
	return "MONITOR " + action + " " + target + "\r\n";
}

string IRC::Message(string target, string body)
{
	return "PRIVMSG " + target + " :" + body + "\r\n";
}

string IRC::Notice(string target, string body)
{
	return "NOTICE " + target + " :" + body + "\r\n";
}

string IRC::Join(string target)
{
	return "JOIN " + target + "\r\n";
}

string IRC::Part(string target, string reason)
{
	if (reason == "")
		return "PART " + target + "\r\n";
	else
		return "PART " + target + " :" + reason + "\r\n";
}

string IRC::Quit(string message)
{
	if (message == "")
		return "QUIT :Program Exit\r\n";
	else
		return "QUIT :" + message + "\r\n";
}

string IRC::Whois(string name)
{
	if (name.length() != 0 && (name.at(0) == '+' || name.at(0) == '@'))
		name = name.c_str() + 1;
	return "WHOIS " + name + "\r\n";
}

string IRC::Names(string object)
{
	return "NAMES " + object + "\r\n";
}

string IRC::Nick(string object)
{
	return "NICK " + object + "\r\n";
}

string IRC::Kick(string target, string channel, string reason)
{
	if (reason == "")
		return "KICK " + channel + " " + target + "\r\n";
	else
		return "KICK " + channel + " " + target + " :" + reason + "\r\n";
}

string IRC::Who(string channel, string arguments)
{
	if (arguments == "")
		return "WHO " + channel + "\r\n";
	else
		return "WHO " + channel + " " + arguments + "\r\n";
}

string IRC::Mode(string target, string mode)
{
	if (mode == "")
		return "MODE " + target + "\r\n";
	else
		return "MODE " + target + " " + mode + "\r\n";
}


string IRC::Topic(string channel, string newtopic)
{
	return "TOPIC " + channel + " :" + newtopic + "\r\n";
}
