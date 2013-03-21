#include "EventHandler.h"

EventHandler::EventHandler(BotCore& b) : bot(b), eventcounter(0)
{
	running = true;
	handlerchanged = false;
	pthread_create(&eventthread, NULL, EventHandler::DoProcessEventQueue, (void *) this);
}

EventHandler::~EventHandler()
{
	running = false;
	++eventcounter;
	pthread_join(eventthread, NULL);
}

void EventHandler::Subscribe(set<string> subscribeto, Handler handler, void * object)
{
	subscriberlock.Lock(); // lock subscriber mutex
	althandlermap[handler][object].insert(subscribeto.begin(), subscribeto.end());
	if (althandlermap[handler][object].find("*") != althandlermap[handler][object].end())
	{
		althandlermap[handler][object].clear();
		althandlermap[handler][object].insert("*");
	}
	handlerchanged = true;
	subscriberlock.Unlock(); // unlock subscriber mutex
	return;
}

void EventHandler::Unsubscribe(Handler handler, void * object)
{
	subscriberlock.Lock(); // lock subscriber mutex
	althandlermap[handler].erase(object);
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
	if (handlerchanged) handlermap = althandlermap; // this isn't very efficient but it should do
	handlerchanged = false;
	subscriberlock.Unlock(); // lock subscriber mutex
}

void EventHandler::HandleEvents()
{
	eventqueuelock.Lock(); // lock eventqueue mutex
	if (eventqueue.size() == 0) return;
	Event e = eventqueue.front();
	eventqueue.pop();
	eventqueuelock.Unlock(); // unlock eventqueue mutex

	NormalizeSubscriptions();

	for (map<Handler, map<void *, set<string> > >::iterator i = handlermap.begin(); i != handlermap.end(); ++i)
	{
		for (map<void *, set<string> >::iterator j = i->second.begin(); j != i->second.end(); ++j)
		{
			if (IsSubscribed(e.name, j->second))
				(i->first)(j->first, e);
		}
	}

	return;
}

bool EventHandler::IsSubscribed(string subject, set<string> subscriptions)
{
	subscriberlock.Lock(); // lock subscriber mutex
	return (subscriptions.find("*") != subscriptions.end()) || (subscriptions.find(subject) != subscriptions.end());
	subscriberlock.Unlock(); // lock subscriber mutex
}

