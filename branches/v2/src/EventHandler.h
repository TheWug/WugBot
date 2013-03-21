#ifndef _EVENT_HANDLER_H_
#define _EVENT_HANDLER_H_

#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>

#include"BotTime.h"
#include "Concurrency.h"

using namespace std;

class BotCore;

class EventHandler
{
public:
	class Event // EventHandler::Event 
	{
	public:
		string name;
		mtime_t timestamp;
		vector<string> data;
	};

	typedef void (*Handler)(void *, Event e);

private:
	// bot reference
	BotCore& bot;

	// handler stuff
	map<Handler, map<void *, set<string> > > handlermap;
	map<Handler, map<void *, set<string> > > althandlermap;
	bool handlerchanged;

	// event queue
	queue<Event> eventqueue;

	// locks and signals
	Mutex eventqueuelock;
	Mutex subscriberlock;
	Semaphore eventcounter;

	// main event handler thread
	pthread_t eventthread;

	// helper functions
	void NormalizeSubscriptions();

	// thread functions
	void ProcessEventQueue();
	void HandleEvents();
	static void * DoProcessEventQueue(void * object);

	bool running;

public:
	EventHandler(BotCore& b);	// creates instance, starts distribution thread (which immediately blocks)
	~EventHandler();		// destroys instance, which stops distribution thread safely

	// event processing
	void Subscribe(set<string> subscribeto, Handler handler, void * object);
	void Unsubscribe(Handler handler, void * object);
	void Raise(Event e);

	// helper functions
	bool IsSubscribed(string subject, set<string> subscriptions);
};

#endif
