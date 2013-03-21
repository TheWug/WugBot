#ifndef _FORWARDER_H_
#define _FORWARDER_H_

#include "dcmd.h"

class Forwarder : public dcmd
{
	string subject;
	string target;
	bool channel;

	public:
	Forwarder(string newsubj, string newtar, IRCBot& bot);

	string GetSubject();
	string GetTarget();

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
};

#endif
