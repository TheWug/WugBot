#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "BotTime.h"

using namespace std;

enum Loglevel
{
	IRC_MESSAGE,
	IRC_NOTICE,
	IRC_OTHER,
	DEBUG,
	INFO,
	WARNING,
	ERROR,
	CRITICAL
};

class Log
{
private:
	ofstream logfile;

public:
	Log(string filename);
	~Log();

	void Redirect(string filename);
	void Close();

	void Message(string message, string speaker);
	void Notice(string message, string speaker);
	void Irc(string message, string speaker);
	void Put(Loglevel level, string message, mtime_t arrived = 0);

	void MkDir(string dirname, struct stat *s);

	static string GetLogLevel(Loglevel l);
};

#endif
