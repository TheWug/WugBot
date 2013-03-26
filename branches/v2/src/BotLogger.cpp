#include "BotLogger.h"
#include "StringUtil.h"

string BotLogger::SYS = "SYSTEM";
string BotLogger::DBG = "DEBUG";
string BotLogger::MOD = "MODULE";
string BotLogger::CMD = "COMMAND";
string BotLogger::RAW = "RAWIO";

BotLogger::BotLogger()
{
}

BotLogger::~BotLogger()
{
	map<string, Log *>::iterator i;
	GetLog("SYSTEM").Put(INFO, "BotLogger: Closing all logs");
	for (i = loglist.begin(); i != loglist.end(); ++i)
	{
		delete i->second;
	}
	return;
}

Log& BotLogger::GetLog(string identifier)
{
	identifier = StringUtil::StripWS(identifier);
	Log * log = loglist[identifier];
	if (log == NULL)
	{
		log = new Log(identifier + ".log");
		loglist[identifier] = log;
	}
	return *log;
}

void BotLogger::CloseLog(string identifier)
{
	identifier = StringUtil::StripWS(identifier);
	Log * log = loglist[identifier];
	if (log != NULL) delete log;
	loglist.erase(identifier);
	return;
}

void BotLogger::RecreateLog(string identifier, string filename)
{
	identifier = StringUtil::StripWS(identifier);
	CloseLog(identifier); // closes log and removes it from log table (if it exists)
	loglist[identifier] = new Log(filename);
	return;
}
