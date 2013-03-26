#ifndef _IRC_PROCESSOR_
#define _IRC_PROCESSOR_

#include <string>
#include <set>

#include "EventHandler.h"

class BotCore;

using namespace std;

class IRCProcessor
{
public:
	class IRCSender
	{
	public:
		IRCSender();
		IRCSender(string full_mask);

		void Set(string full_mask);
		string Get();

		string nickname;
		string ident;
		string hostname;
	};

	struct IRCUser
	{
	public:
		string nickname;
		string ident;
		string hostname;
		string svcacct;

		set<string> channels;
	};

	struct IRCChannel
	{
	public:
		string name;
		// IRCChanMode mode;
		
		set<string> users;
	};

	IRCProcessor(BotCore& b);

	string GetLogMessage(EventHandler::Event& e);
	void LogProcess(EventHandler::Event& e);
	void RawLog(EventHandler::Event& e);

	string CreateIRCLine(vector<string>& components);
	vector<string> ParseIRCLine(string line);
	void AnalyzeAndClassify(EventHandler::Event &e);
	bool IsChannel(string s);

	static void WelcomeHandler(void *s, EventHandler::Event e);
	static void IOHandler(void *s, EventHandler::Event e);

	void SubscribeToEvents();

	BotCore& bot;

	set<string> mychannels;
};

#endif
