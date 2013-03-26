#include "Log.h"

#include <iostream>

#include "StringUtil.h"

Log::Log(string filename)
{
	Redirect(filename);
}

Log::~Log()
{
	Close();
}

void Log::Redirect(string filename)
{
	Put(INFO, "Log: Redirecting log (new log: " + filename + ")");
	if (logfile.is_open()) logfile.close();
	struct stat s;
	if (filename.find("/") != string::npos) MkDir(filename.substr(0, filename.find("/")), &s);
	if (filename != "") logfile.open(filename.c_str(), ios_base::out | ios_base::app);
	Put(INFO, "Log: Opened log");
}

void Log::Close()
{
	Put(INFO, "Log: Closing log");
	if (logfile.is_open()) logfile.close();
}

void Log::Put(Loglevel level, string message, mtime_t arrived)
{
	if (!logfile.is_open()) return;

	if (arrived == 0) arrived = BotTime::GetCurrentTimeMillis();
	string timestamp = StringUtil::FormatTime("%F %T.%nnn", arrived);
	string logline = timestamp + " " + GetLogLevel(level) + message;

	cout << logline << endl;
	logfile << logline << endl;
}

void Log::MkDir(string dirname, struct stat *s)
{
	if (dirname.find("/") != string::npos) MkDir(dirname.substr(0, dirname.find("/")), s);
	if (stat(dirname.c_str(), s)) if (mkdir(dirname.c_str(), S_IRWXU)) cout << "MKDIR FAILED" << endl;
}

string Log::GetLogLevel(Loglevel l)
{	
	static string names[8] =
	{
		"",
		"",
		"",
		"[DEBUG]    ",
		"[INFO]     ",
		"[WARNING]  ",
		"[ERROR]    ",
		"[CRITICAL] "
	};
	if (l >= 0 && l <= CRITICAL) return names[l];
	else return "[???]      ";
}
