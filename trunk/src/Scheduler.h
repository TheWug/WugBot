#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include "Bottime.h"
#include "EventActions.h"
#include <iostream>
#include <pthread.h>
#include <queue>
#include <set>

using namespace std;

typedef unsigned int event_t;

class Scheduler;

class Event
{
public:
	Event(float seconds, action * a);
	Event(float seconds, int repetitions, action * a);

	bool Due() const;
	void DoEvent();
	mtime_t Countdown() const;
	void Next();
	void Cancel();
	bool ShouldReschedule();
	void SetID(Scheduler * s);
	event_t GetID() const;

	bool operator > (const Event& other) const;

private:
	action * onevent;
	mtime_t next;
	mtime_t interval;
	int repeats;
	event_t id;
};

class Scheduler
{
public:
	Scheduler();
	~Scheduler();

	volatile bool threadrunning;

	bool StartEventLoop();
	void StopEventLoop();

	event_t AddEvent(Event e);
	void RemoveEvent(event_t id);

	event_t NextAvailableID();

private:
	// in-thread functions
	static void * DoRunEventLoop(void * arg);
	void RunEventLoop();
	void SleepUntilNextEvent();

	// out-thread support functions
	void QueueLock();
	void QueueUnlock();
	void NotifyEventThread();

	// synchronization and threading primitives
	pthread_t eventthread;
	pthread_mutex_t queuelock;
	pthread_cond_t eventclock;

	// event queue
	priority_queue<Event, vector<Event>, greater<Event> > eventqueue;
	set<event_t> cancelled;
	event_t next;
};

#endif
