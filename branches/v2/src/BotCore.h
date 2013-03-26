#ifndef _BOTCORE_H_
#define _BOTCORE_H_

#include "BotLogger.h"
#include "EventHandler.h"
#include "NetManager.h"
#include "Properties.h"
#include "Scheduler.h"
#include "HouseKeeper.h"
#include "IRCProcessor.h"
#include "Concurrency.h"

class BotCore
{
private:
	BotLogger log;
	EventHandler * evt;
	NetManager * net;
	Properties * prop;
	Scheduler * sch;
	HouseKeeper * keep;
	IRCProcessor * irc;

	void Init(string config);
	void Deinit();

	Semaphore running;

	static void RuntimeHandler(void * self, EventHandler::Event e);

	void SubscribeSelf();

public:
	BotCore();
	~BotCore();

	BotLogger& BotLog();
	EventHandler& BotEvents();
	NetManager& BotNet();
	Scheduler& BotSched();
	Properties& BotProp();
	HouseKeeper& BotHouse();
	IRCProcessor& BotIRC();

	void StartBot(bool synchronous, string config);
	void Wait();
	void StopBot(string reason);
};

#endif
