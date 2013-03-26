#include "BotCore.h"

#include <stdexcept>

#include "CommonEvents.h"
#include "StringUtil.h"

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

NetManager& BotCore::BotNet()
{
	return *net;
}

Scheduler& BotCore::BotSched()
{
	return *sch;
}

Properties& BotCore::BotProp()
{
	return *prop;
}

HouseKeeper& BotCore::BotHouse()
{
	return *keep;
}

IRCProcessor& BotCore::BotIRC()
{
	return *irc;
}

void BotCore::StartBot(bool synchronous, string config)
{
	Init(config);
	if (synchronous) Wait();
	
}

void BotCore::StopBot(string reason)
{
	log.GetLog(BotLogger::SYS).Put(INFO, "BotCore::StopBot: received external request to shut down.");
	EventHandler::Event e;
	e.name = CommonEvents::BOT_DIE;
	e.data.push_back(reason);
	BotEvents().Raise(e);
	Wait();
}

void BotCore::Wait()
{
	log.GetLog(BotLogger::DBG).Put(INFO, "BotCore::Wait: controlling thread waiting for lock release");
	--running; // semaphore lock, waits for increment.  if increment has already happened, don't block
	log.GetLog(BotLogger::DBG).Put(INFO, "BotCore::Wait: controlling awoke, bot shutdown starting");
	Deinit();
}

void BotCore::Init(string config)
{
	log.GetLog(BotLogger::SYS).Put(INFO, "BotCore: bot initialization started");

	net = new NetManager(*this);
	prop = new Properties(config, *this);
	sch = new Scheduler(*this);
	evt = new EventHandler(*this);
	keep = new HouseKeeper(*this);
	irc = new IRCProcessor(*this);

	BotNet().Start();
	BotSched().Start();

	SubscribeSelf();
	BotNet().SubscribeToEvents();
	BotProp().SubscribeToEvents();
	BotSched().SubscribeToEvents();
	BotHouse().SubscribeToEvents();
	BotIRC().SubscribeToEvents();

	log.GetLog(BotLogger::SYS).Put(INFO, "BotCore: bot initialization finished");

	EventHandler::Event bot_start;
	bot_start.name = CommonEvents::BOT_STARTUP;
	BotEvents().Raise(bot_start);
}

void BotCore::Deinit()
{
	log.GetLog(BotLogger::SYS).Put(INFO, "BotCore: bot shutdown started");

	BotNet().Stop();
	BotSched().Stop();

	delete evt; // this should be deleted first, deleting it stops it from distributing events to possibly freed objects
	delete net;
	delete prop;
	delete sch;
	delete keep;
	delete irc;

	log.GetLog(BotLogger::SYS).Put(INFO, "BotCore: bot shutdown finished.");
}

void BotCore::RuntimeHandler(void * s, EventHandler::Event e)
{
	BotCore * self = (BotCore *) s;
	if (e.name == CommonEvents::BOT_SIGNAL)
	{
		self->BotLog().GetLog(BotLogger::SYS).Put(WARNING, "Received signal: " + e.data[0] + " (" + e.data[1] + ")");
		try
		{
			int signum = StringUtil::ToInt32(e.data[0]);
			if (signum == 2)
			{
				EventHandler::Event response;
				response.name = CommonEvents::BOT_DIE;
				self->BotEvents().Raise(response);
			}
		}
		catch (domain_error &err)
		{
			self->BotLog().GetLog(BotLogger::DBG).Put(ERROR, "BotCore::RuntimeHandler: Could not parse integer from string: " + e.data[0]);
		}
	}
	else if (e.name == CommonEvents::BOT_DIE)
	{
		EventHandler::Event response;
		response.name = CommonEvents::BOT_SAVEALL;
		self->BotEvents().Raise(response);
		response = EventHandler::Event();
		response.name = CommonEvents::BOT_SHUTDOWN;
		self->BotEvents().Raise(response);
	}
	else if (e.name == CommonEvents::BOT_SHUTDOWN)
	{
		EventHandler::Event response;
		response.name = CommonEvents::BOT_FIN;
		self->BotEvents().Raise(response);
	}
	else if (e.name == CommonEvents::BOT_FIN)
	{
		EventHandler::Event response;
		response.name = CommonEvents::BOT_TERMINATE;
		self->BotEvents().Raise(response);
	}
	else if (e.name == CommonEvents::BOT_TERMINATE)
	{
		self->log.GetLog(BotLogger::DBG).Put(INFO, "BotCore::RuntimeHandler: releasing control of shutdown process to controlling thread");
		++self->running;
	}
	return;
}

void BotCore::SubscribeSelf()
{
	log.GetLog(BotLogger::DBG).Put(INFO, "BotCore::SubscribeSelf: subscribing to events 'bot::*'");
	set<string> myevents;
	myevents.insert(CommonEvents::BOT_ALL);
	BotEvents().Subscribe(myevents, RuntimeHandler, this);
}
