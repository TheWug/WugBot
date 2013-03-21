#ifndef _KICKWATCHER_H_
#define _KICKWATCHER_H_

#include "dcmd.h"

class KickWatcher : public dcmd
{
	string subject, channel;

	public:
	KickWatcher(string chan, string subj, Hostname speaker, string target, IRCBot& bot, string message);

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
};

#endif
