#ifndef _DCMD_H_
#define _DCMD_H_

#include <string>

#include "Hostname.h"

using namespace std;
class IRCBot;

class dcmd
{
	protected:
	Hostname originalspeaker;
	string originaltarget;
	IRCBot& bot;

	public:
	dcmd(Hostname speaker, string target, IRCBot& bot);
	virtual ~dcmd() {}

	virtual bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag) = 0;
};

#endif
