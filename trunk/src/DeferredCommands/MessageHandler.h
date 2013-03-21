#ifndef _MESSAGEHANDLER_H_
#define _MESSAGEHANDLER_H_

#include <map>

#include "dcmd.h"

class MessageHandler : public dcmd
{
	private:
	map<string, map<string, map<string, string> > > watchlist;

	public:
	MessageHandler(IRCBot& b) : dcmd(Hostname(""), "", b) {}

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);

	void SetCommand(string message, string target, string subject, string command);
	void ClearCommand(string message, string subject, string target);
	string Replace(Hostname speaker, string target, string message, string command);
};

#endif
