#include "HouseKeeper.h"

#include "BotCore.h"
#include "CommonEvents.h"

HouseKeeper::HouseKeeper(BotCore& b) : bot(b)
{
}

void HouseKeeper::StartupHandler(void *x, EventHandler::Event e)
{
	HouseKeeper * self = (HouseKeeper *) x;
	if (self->bot.BotProp().GetProperty("discarddebug", "GENERAL", 1)) self->bot.BotLog().GetLog(BotLogger::DBG).Redirect("/dev/null");
}

void HouseKeeper::SubscribeToEvents()
{
	set<string> myevents;
	myevents.insert(CommonEvents::PROP_READY);
	myevents.insert(CommonEvents::PROP_LOADED);
	bot.BotEvents().Subscribe(myevents, HouseKeeper::StartupHandler, this);
}
