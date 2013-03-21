#ifndef _FLOODENGINE_H_
#define _FLOODENGINE_H_

#include <string>

#include "FloodEvents.h"
#include "Messenger.h"
#include "Hostname.h"

class IRCBot;
class FloodProtectModule;
class ChannelSettings;

class FloodEngine
{
public:
	virtual void Analyze(FloodProtectModule * base, Hostname speaker, string target, Messenger respondto, IRCBot& bot);
	void HandleNewEvent(FloodProtectModule * base, FloodType f, mtime_t timestamp, Hostname speaker, string target, string message, string repealmessage);
};

class MessageFloodEngine : public FloodEngine
{
public:
	void Analyze(FloodProtectModule * base, Hostname speaker, string target, Messenger respondto, IRCBot& bot);

	float CruiseControlQuotient(string s);
	mtime_t FigureOutDelta(TimeLimitedQueue<string>& msgs, ChannelSettings& settings);
};

class PingFloodEngine : public FloodEngine
{
public:
	void Analyze(FloodProtectModule * base, Hostname speaker, string target, Messenger respondto, IRCBot& bot);
};

class NickFloodEngine : public FloodEngine
{
public:
	void Analyze(FloodProtectModule * base, Hostname speaker, string target, Messenger respondto, IRCBot& bot);
	mtime_t FigureOutDelta(TimeLimitedQueue<Nothing>& msgs, ChannelSettings& settings);
};

class CycleFloodEngine : public FloodEngine
{
public:
	void Analyze(FloodProtectModule * base, Hostname speaker, string target, Messenger respondto, IRCBot& bot);
	mtime_t FigureOutDelta(TimeLimitedQueue<Nothing>& msgs, ChannelSettings& settings);
};

class ChannelFloodEngine : public FloodEngine
{
public:
	void Analyze(FloodProtectModule * base, Hostname speaker, string target, Messenger respondto, IRCBot& bot);
};

#endif
