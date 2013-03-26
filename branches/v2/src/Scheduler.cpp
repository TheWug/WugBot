#include "Scheduler.h"

#include <iostream>

#include "BotCore.h"
#include "CommonEvents.h"
#include "StringUtil.h"

Scheduler::Occurrence::Occurrence(float seconds, EventHandler::Event e) : event(e)
{
	interval = ((mtime_t) (seconds * 1000));
	next = BotTime::GetCurrentTimeMillis() + interval;
	repeats = 1;
	return;
}

Scheduler::Occurrence::Occurrence(float seconds, int repetitions, EventHandler::Event e) : event(e)
{
	interval = ((mtime_t) (seconds * 1000));
	next = BotTime::GetCurrentTimeMillis() + interval;
	repeats = repetitions;
	return;
}

bool Scheduler::Occurrence::IsDue() const
{
	return (next - BotTime::GetCurrentTimeMillis()) <= 0LL;
}

void Scheduler::Occurrence::Mark()
{
	if (repeats > 0)
	{
		--repeats;
	}
	next += interval;
	return;
}

bool Scheduler::Occurrence::ShouldReschedule()
{
	return repeats;
}

mtime_t Scheduler::Occurrence::Countdown() const
{
	long long t = next - BotTime::GetCurrentTimeMillis();
	return (t < 0LL) ? 0LL : t;
}

void Scheduler::Occurrence::SetID(Scheduler * s)
{
	id = s->NextAvailableID();
	return;
}

Scheduler::occur_t Scheduler::Occurrence::GetID() const
{
	return id;
}

bool Scheduler::Occurrence::operator > (const Occurrence& other) const
{
	return (next > other.next);
}

Scheduler::Scheduler(BotCore& b) : bot(b)
{
	next = 0;
	threadrunning = false;
	bot.BotLog().GetLog(BotLogger::SYS).Put(INFO, "Scheduler::Scheduler: initialized scheduler and deferred event processing system");
	return;
}

Scheduler::~Scheduler()
{
	return;
}

Scheduler::occur_t Scheduler::AddOccurrence(Occurrence o)
{
	o.SetID(this);
	queuelock.Lock();
	eventqueue.push(o);
	occurrences.insert(o.GetID());
	queuelock.Unlock();
	
	++eventclock;
	return o.GetID();
}

void Scheduler::RemoveOccurrence(Scheduler::occur_t id)
{
	queuelock.Lock();
	occurrences.erase(id);
	queuelock.Unlock();
	return;
}

Scheduler::occur_t Scheduler::NextAvailableID()
{
	return ++next;
}

void Scheduler::Start()
{
	threadrunning = true;
	pthread_create(&eventthread, NULL, Scheduler::DoRunEventLoop, (void *) this);
	bot.BotLog().GetLog(BotLogger::SYS).Put(INFO, "Scheduler::Start: scheduler ready.");
	return;
}

void Scheduler::Stop()
{
	threadrunning = false;
	++eventclock;
	pthread_join(eventthread, NULL);
	bot.BotLog().GetLog(BotLogger::SYS).Put(INFO, "Scheduler::Stop: scheduler shutdown.");
	return;
}

/* Thread Operations */
void * Scheduler::DoRunEventLoop(void * arg)
{
	Scheduler * self = ((Scheduler *) arg);
	self->queuelock.Lock();
	self->RunEventLoop();
	self->queuelock.Unlock();
	return NULL;
}

void Scheduler::RunEventLoop()
{
	while (threadrunning)
	{
		SleepUntilNextEvent();

		servicelock.Lock();
		if (!servicequeue.empty())
		{
			servicequeue.pop();
			servicelock.Unlock();
			++serviceready;
			servicecond.Wait(queuelock);
			continue;
		}
		servicelock.Unlock();
		
		ProcessOccurrence();
	}
	return;
}

void Scheduler::SleepUntilNextEvent()
{
	queuelock.Unlock();
	if (eventqueue.empty()) --eventclock;
	else
	{
		timespec t = BotTime::GetMillisAsTimespec(eventqueue.top().Countdown() + BotTime::GetCurrentTimeMillis());
		eventclock -= t;
	}
	queuelock.Lock();
	return;
}

void Scheduler::PauseSchedulerThread()
{
	bot.BotLog().GetLog(BotLogger::DBG).Put(INFO, "Scheduler::PauseSchedulerThread: attempting to stop scheduler thread...");
	servicelock.Lock();
	servicequeue.push(".");
	servicelock.Unlock();
	++eventclock;
	--serviceready;
	queuelock.Lock();
	bot.BotLog().GetLog(BotLogger::DBG).Put(INFO, "Scheduler::PauseSchedulerThread: scheduler thread paused.");
	return;
}

void Scheduler::UnpauseSchedulerThread()
{
	servicecond.Signal();
	queuelock.Unlock();
	bot.BotLog().GetLog(BotLogger::DBG).Put(INFO, "Scheduler::UnpauseSchedulerThread: scheduler thread unpaused.");
	return;
}

void Scheduler::ProcessOccurrence()
{
	if (eventqueue.empty()) return;

	Occurrence o = eventqueue.top();

	if (!o.IsDue()) return;
	eventqueue.pop();

	if (occurrences.find(o.GetID()) == occurrences.end())
	{
		bot.BotLog().GetLog(BotLogger::DBG).Put(WARNING, "Scheduler::ProcessOccurrence: Processed cancelled occurrence (dropped it)");
		return;
	}

	bot.BotLog().GetLog(BotLogger::DBG).Put(INFO, "Scheduler::ProcessOccurrence: Processing occurrence " + StringUtil::FromInt32(o.GetID()));
	queuelock.Unlock();
	bot.BotEvents().Raise(o.event);
	queuelock.Lock();

	o.Mark();
	if (o.ShouldReschedule()) eventqueue.push(o);
	else occurrences.erase(o.GetID());
	return;
}

void Scheduler::ScheduleHandler(void * s, EventHandler::Event e)
{
	return;
}

void Scheduler::SubscribeToEvents()
{
	return; // I can think of no conceivable reason for this component to receive events at this stage in development

	set<string> myevents;
	myevents.insert(CommonEvents::BOT_STARTUP);
	myevents.insert(CommonEvents::BOT_SAVEALL);
	bot.BotEvents().Subscribe(myevents, ScheduleHandler, this);
}
