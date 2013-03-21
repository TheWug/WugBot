#include "Log.h"

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
	if (filename != "") logfile.open(filename.c_str(), ios_base::out | ios_base::app);
	Put(INFO, "Log: Opened log");
}

void Log::Close()
{
	Put(INFO, "Log: Closing log");
	if (logfile.is_open()) logfile.close();
}

void Log::Message(string message, string speaker)
{
	Put(IRC_MESSAGE, "<" + speaker + "> " + message);
}

void Log::Notice(string message, string speaker)
{
	Put(IRC_NOTICE, "-" + speaker + "- " + message);
}

void Log::Irc(string message, string speaker)
{
	Put(IRC_OTHER, "== " + speaker + ": " + message);
}

void Log::Put(Loglevel level, string message, mtime_t arrived)
{
	if (!logfile.is_open()) return;

	if (arrived == 0) arrived = BotTime::GetCurrentTimeMillis();
	string timestamp = StringUtil::FormatTime("dd-mm-yyyy HH:MM:SS.xxx", arrived);
	string logline = timestamp + " " + GetLogLevel(level) + message;

	logfile << logline << endl;
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
		"[WARN]     ",
		"[ERROR]    ",
		"[CRITICAL] "
	};
	if (l >= 0 && l <= CRITICAL) return names[l];
	else return "[???]      ";
}
