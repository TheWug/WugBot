#ifndef _BOTCORE_H_
#define _BOTCORE_H_

#include "BotLogger.h"
#include "EventHandler.h"
#include "NetManager.h"
#include "Properties.h"
#include "Concurrency.h"

class BotCore
{
private:
	BotLogger log;
	EventHandler * evt;
	NetManager * net;
	Properties * prop;

	void Init(string config);
	void Deinit();

	Semaphore running;

public:
	BotCore();
	~BotCore();

	BotLogger& BotLog();
	EventHandler& BotEvents();

	void StartBot(bool synchronous, string config);
	void Wait();
	void StopBot(string reason);
};

#endif
