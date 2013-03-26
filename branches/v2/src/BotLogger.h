#ifndef _BOT_LOGGER_H_
#define _BOT_LOGGER_H_

#include <map>
#include <string>
#include <iostream>

#include "Log.h"

using namespace std;

class BotLogger
{
private:
	map<string, Log *> loglist;
	
public:
	BotLogger();
	~BotLogger();

	Log& GetLog(string identifier);
	void CloseLog(string identifier);
	void RecreateLog(string identifier, string filename);

	static string SYS, DBG, MOD, CMD, RAW;
};

#endif
