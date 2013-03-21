#include <stdlib.h>
#include <iostream>
#include <sstream>

#include "BotInfo.h"

BotInfo::BotInfo()
{
	usedaemon = true;
	setnickname = false;
	setaltnickname = false;
	setpassword = false;
	setserver = false;
	setport = false;
	setservicesaccount = false;
	setrealname = false;
	setusername = false;
	setownername = false;
	setconnectcommands = false;
	setcommandprefix = false;
	setreadonly = false;
	setlogchannel = false;
	setfloodeventweight = false;
	setfloodmaxweight = false;
}

void BotInfo::SetReadOnly(string ro)
{
	readonly = (ro == "true" || ro == "yes" || ro == "on" || ro == "y" || ro == "t" || ro == "1");
	setreadonly = true;
}

bool BotInfo::GetReadOnly() throw (string)
{
	if (setreadonly)
		return readonly;
	return false;
}

void BotInfo::SetNick(string n)
{
	setnickname = true;
	nickname = n;
}

void BotInfo::SetUseDaemon(bool b)
{
	usedaemon = b;
}

void BotInfo::SetAltNick(string n)
{
	setaltnickname = true;
	altnickname = n;
}

void BotInfo::SetPass(string pass)
{
	setpassword = true;
	password = pass;
}

void BotInfo::SetAccount(string acct)
{
	setservicesaccount = true;
	servicesaccount = acct;
}

void BotInfo::SetRealname(string r)
{
	setrealname = true;
	realname = r;
}

void BotInfo::SetUser(string user)
{
	setusername = true;
	username = user;
}

void BotInfo::SetOwner(string owner)
{
	setownername = true;
	ownername = owner;
}

void BotInfo::SetConnectCommands(string cc)
{
	setconnectcommands = true;
	connectcommands = cc;
}

void BotInfo::SetServer(string s)
{
	setserver = true;
	int pos = s.rfind(':');
	server = s.substr(0, pos);
	string p = s.substr(pos + 1);
	int temp_port = 0;
	temp_port = atoi(p.c_str());
	if (temp_port != 0)
	{
		setport = true;
		port = temp_port;
	}
}

void BotInfo::SetCommandPrefix(string prefix)
{
	commandprefix = prefix;
	setcommandprefix = true;
}

void BotInfo::SetFloodMinInterval(string prefix)
{
	istringstream s(prefix);
	s >> floodminint;
	if (s) setfloodminint = true;
}

void BotInfo::SetFloodEventWeight(string prefix)
{
	istringstream s(prefix);
	s >> floodeventweight;
	if (s) setfloodeventweight = true;
}

void BotInfo::SetFloodMaxWeight(string prefix)
{
	istringstream s(prefix);
	s >> floodmaxweight;
	if (s) setfloodmaxweight = true;
}

void BotInfo::SetLogChannel(string logchan)
{
	logchannel = logchan;
	setlogchannel = true;
}

bool BotInfo::GetUseDaemon() throw (string)
{
	return usedaemon;
}

string BotInfo::GetNick() throw (string)
{
	if (!setnickname)
		throw string("Nickname not specified.");
	return nickname;
}

string BotInfo::GetAltNick() throw (string)
{
	if (!setaltnickname)
		return nickname + "_";
	return altnickname;
}

string BotInfo::GetPass() throw (string)
{
	if (!setpassword)
		return "";
	return password;
}

string BotInfo::GetAccount() throw (string)
{
	if (!setservicesaccount)
		return GetNick();
	return servicesaccount;
}

string BotInfo::GetRealname() throw (string)
{
	if (!setrealname)
	{
		return GetOwner() + "'s bot";
	}
	return realname;
}

string BotInfo::GetUser() throw (string)
{
	if (!setusername)
		return GetOwner();
	return username;
}

string BotInfo::GetOwner() throw (string)
{
	if (!setownername)
		return GetAccount();
	return ownername;
}

string BotInfo::GetConnectCommands() throw (string)
{
	if (!setconnectcommands)
		return "IRCBot.connect";
	return connectcommands;
}

string BotInfo::GetServer() throw (string)
{
	if (!setserver)
		throw string("No server specified.");
	return server;
}

int BotInfo::GetPort() throw (string)
{
	if (!setport)
		return 6667;
	return port;
}

string BotInfo::GetCommandPrefix() throw (string)
{
	if (!setcommandprefix)
		return ";";
	return commandprefix;
}

float BotInfo::GetFloodMinInterval() throw (string)
{
	if (!setfloodminint)
		return 0;
	return floodminint;
}

float BotInfo::GetFloodEventWeight() throw (string)
{
	if (!setfloodeventweight)
		return 1;
	return floodeventweight;
}

float BotInfo::GetFloodMaxWeight() throw (string)
{
	if (!setfloodmaxweight)
		return 8;
	return floodmaxweight;
}

string BotInfo::GetLogChannel() throw (string)
{
	if (!setlogchannel)
		return "";
	return logchannel;
}

bool BotInfo::HasEnoughInfo()
{
	return (setnickname && setserver);
}


