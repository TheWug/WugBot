#include "BotCore.h"

#include "CommonEvents.h"

BotCore::BotCore() : running(0)
{
}

BotCore::~BotCore()
{
}

BotLogger& BotCore::BotLog()
{
	return log;
}

EventHandler& BotCore::BotEvents()
{
	return *evt;
}

void BotCore::StartBot(bool synchronous, string config)
{
	Init(config);
	if (synchronous) Wait();
	
}

void BotCore::StopBot(string reason)
{
	EventHandler::Event e;
	e.name = CommonEvents::BOT_DIE;
	e.timestamp = BotTime::GetCurrentTimeMillis();
	e.data.push_back(reason);
	BotEvents().Raise(e);
	Wait();
}

void BotCore::Wait()
{
	--running; // semaphore lock, waits for increment.  if increment has already happened, don't block
	Deinit();
}

void BotCore::Init(string config)
{
	log.GetLog(BotLogger::SYS).Put(INFO, "BotCore: bot initialization started");
	evt = new EventHandler(*this); // this should be the first subobject initialized since most of the others depend on it
	net = new NetManager(*this);
	prop = new Properties(config, *this);
	log.GetLog(BotLogger::SYS).Put(INFO, "BotCore: bot initialization finished");
}

void BotCore::Deinit()
{
	log.GetLog(BotLogger::SYS).Put(INFO, "BotCore: bot shutdown started");
	delete net;
	delete prop;
	delete evt;
	log.GetLog(BotLogger::SYS).Put(INFO, "BotCore: bot shutdown finished.");
}
