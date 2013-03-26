#ifndef _HOUSEKEEPER_H_
#define _HOUSEKEEPER_H_

#include "EventHandler.h"

class BotCore;

class HouseKeeper
{
private:
	BotCore& bot;

	static void StartupHandler(void *x, EventHandler::Event e);
public:
	HouseKeeper(BotCore& b);

	void SubscribeToEvents();
};

#endif
