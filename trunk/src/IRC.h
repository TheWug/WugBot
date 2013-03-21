#include <string>

#include "Hostname.h"

using namespace std;

class ServerProperties;

class IRC
{
	IRC() {}

	public:
	static string Monitor(string action, string target);
	static string Message(string target, string body);
	static string Notice(string target, string body);
	static string Join(string target);
	static string Part(string target, string message);
	static string Quit(string message);
	static string Names(string object);
	static string Whois(string object);
	static string Nick(string object);
	static string Kick(string target, string channel, string reason);
	static string Who(string channel, string arguments);
	static string Mode(string target, string mode);
	static string Topic(string channel, string newtopic);

	static bool IsValidNick(ServerProperties& p, string testnick);
	static bool IsValidChannel(ServerProperties& p, string testchan);
};
