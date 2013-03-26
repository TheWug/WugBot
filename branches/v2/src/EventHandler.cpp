#include "EventHandler.h"

#include "BotCore.h"
#include "BotLogger.h"
#include "BotTime.h"

EventHandler::Event::Event()
{
	timestamp = BotTime::GetCurrentTimeMillis();
}

EventHandler::EventHandler(BotCore& b) : bot(b), eventcounter(0)
{
	running = true;
	handlerchanged = false;
	pthread_create(&eventthread, NULL, EventHandler::DoProcessEventQueue, (void *) this);
	bot.BotLog().GetLog(BotLogger::DBG).Put(INFO, "EventHandler::EventHandler: initialized event handler, started event handler thread");
}

EventHandler::~EventHandler()
{
	running = false;
	++eventcounter;
	pthread_join(eventthread, NULL);
	bot.BotLog().GetLog(BotLogger::DBG).Put(INFO, "EventHandler::EventHandler: shutting down event handler, stopped event handler thread");
}

void EventHandler::Subscribe(set<string> sub_to, Handler handler, void * object)
{
	subscriberlock.Lock(); // lock subscriber mutex
	for (set<string>::iterator i = sub_to.begin(); i != sub_to.end(); ++i) althandlermap[handler][object].Add(*i);
	handlerchanged = true;
	subscriberlock.Unlock(); // unlock subscriber mutex
	return;
}

void EventHandler::Unsubscribe(set<string> unsub_from, Handler handler, void * object)
{
	subscriberlock.Lock(); // lock subscriber mutex
	for (set<string>::iterator i = unsub_from.begin(); i != unsub_from.end(); ++i) althandlermap[handler][object].Remove(*i);
	handlerchanged = true;
	subscriberlock.Unlock(); // unlock subscriber mutex
	return;
}

void EventHandler::Raise(Event e)
{
	eventqueuelock.Lock(); // lock eventqueue mutex (can't be locked unless eventsignal is)
	eventqueue.push(e);
	eventqueuelock.Unlock(); // unlock eventqueue mutex
	++eventcounter;
}

void * EventHandler::DoProcessEventQueue(void * object)
{
	((EventHandler *) object)->ProcessEventQueue();
	return NULL;
}

void EventHandler::ProcessEventQueue()
{
	while (running)
	{
		--eventcounter;
		HandleEvents();
	}
	return;
}


void EventHandler::NormalizeSubscriptions()
{
	subscriberlock.Lock(); // lock subscriber mutex
	if (handlerchanged)
	{
		handlermap = althandlermap; // this isn't very efficient but it should do		
		bot.BotLog().GetLog(BotLogger::DBG).Put(INFO, "EventHandler::NormalizeSubscriptions: updated subscription list");
	}
	handlerchanged = false;
	subscriberlock.Unlock(); // lock subscriber mutex
}

void EventHandler::HandleEvents()
{
	eventqueuelock.Lock(); // lock eventqueue mutex
	if (eventqueue.size() == 0) return;
	Event e = eventqueue.front();
	bot.BotLog().GetLog(BotLogger::DBG).Put(INFO, "EventHandler::HandleEvents: Dispatching Event (" + e.name + ")");
	eventqueue.pop();
	eventqueuelock.Unlock(); // unlock eventqueue mutex

	NormalizeSubscriptions();

	for (map<Handler, map<void *, WildcardStringMatcher> >::iterator i = handlermap.begin(); i != handlermap.end(); ++i)
	{
		for (map<void *, WildcardStringMatcher>::iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			if (IsSubscribed(e.name, j->second))
			{
				(i->first)(j->first, e);
			}
		}
	}

	return;
}

bool EventHandler::IsSubscribed(string subject, WildcardStringMatcher& subscriptions)
{
	subscriberlock.Lock(); // lock subscriber mutex
	bool rv = subscriptions.Check(subject);
	subscriberlock.Unlock(); // lock subscriber mutex
	return rv;
}

