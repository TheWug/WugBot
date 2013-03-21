#ifndef _METRICS_H_
#define _METRICS_H_

#include <string>

#include "Hostname.h"
#include "module.h"

using namespace std;

extern "C" module * Factory(IRCBot& bot);

class Metrics : public module
{
	public:
	Metrics(IRCBot& bot);
	virtual ~Metrics();

	void FindUser(string args, Hostname& speaker, string target, Messenger respondto);

	void onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag);
	void onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto);
};

#endif
