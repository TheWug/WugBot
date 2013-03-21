#ifndef _IRC_PROCESSOR_
#define _IRC_PROCESSOR_

#include <string>
#include <set>

using namespace std;

class IRCProcessor
{


public:
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
		IRCChanMode mode;
		
		set<string> users;
	}

	set<string> mychannels;

	
};

#endif
