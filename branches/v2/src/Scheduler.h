#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#include <iostream>
#include <pthread.h>
#include <queue>
#include <set>

#include "BotTime.h"
#include "EventHandler.h"
#include "Concurrency.h"

using namespace std;

class BotCore;

class Scheduler
{
public:
	typedef unsigned int occur_t;

	class Occurrence
	{
	public:
		Occurrence(float seconds, EventHandler::Event e);
		Occurrence(float seconds, int repetitions, EventHandler::Event e);

		bool IsDue() const;
		mtime_t Countdown() const;

		void Mark();
		bool ShouldReschedule();

		void SetID(Scheduler * s);
		Scheduler::occur_t GetID() const;

		bool operator > (const Occurrence& other) const;

		EventHandler::Event event;
		mtime_t next;
		mtime_t interval;
		int repeats;
		Scheduler::occur_t id;
	};

private:
	// in-thread functions
	static void * DoRunEventLoop(void * arg);
	void RunEventLoop();
	void SleepUntilNextEvent();
	void ProcessOccurrence();

	// out-thread support functions
	void PauseSchedulerThread();
	void UnpauseSchedulerThread();

	BotCore& bot;

	// thread and thread control
	pthread_t eventthread;
	volatile bool threadrunning;

	// primary locking mechanisms
	Semaphore eventclock;
	Semaphore serviceready;

	// event queue and lock
	priority_queue<Occurrence, vector<Occurrence>, greater<Occurrence> > eventqueue;
	Mutex queuelock;

	set<occur_t> occurrences;
	Scheduler::occur_t next;

	// service queue, lock, and scheduler thread pause condition
	queue<string> servicequeue;
	Mutex servicelock;
	Condition servicecond;

	static void ScheduleHandler(void * s, EventHandler::Event e);

public:
	Scheduler(BotCore& b);
	~Scheduler();

	void Start();
	void Stop();

	Scheduler::occur_t AddOccurrence(Occurrence o);
	void RemoveOccurrence(occur_t id);

	Scheduler::occur_t NextAvailableID();
	void SubscribeToEvents();
};

#endif
