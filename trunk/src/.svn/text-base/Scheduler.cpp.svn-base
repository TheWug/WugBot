#include "Scheduler.h"
#include <iostream>

Event::Event(float seconds, action * a)
{
	interval = ((mtime_t) (seconds * 1000));
	next = Bottime::GetCurrentTimeMillis() + interval;
	repeats = 1;
	onevent = a;
}

Event::Event(float seconds, int repetitions, action * a)
{
	interval = ((mtime_t) (seconds * 1000));
	next = Bottime::GetCurrentTimeMillis() + interval;
	repeats = repetitions;
	onevent = a;
}

bool Event::Due() const
{
	return (next - Bottime::GetCurrentTimeMillis()) < 0;
}

void Event::Next()
{
	if (repeats > 0)
	{
		--repeats;
	}
	next += interval;
	if (!repeats)
	{
		delete onevent;
		onevent = NULL;
	}
}

void Event::Cancel()
{
	delete onevent;
}

bool Event::ShouldReschedule()
{
	return repeats;
}

void Event::DoEvent()
{
	if (onevent != NULL)
		onevent->DoEvent();
}

mtime_t Event::Countdown() const
{
	mtime_t t = next - Bottime::GetCurrentTimeMillis();
	return (t < 0LL) ? 0LL : t;
}

void Event::SetID(Scheduler * s)
{
	id = s->NextAvailableID();
}

event_t Event::GetID() const
{
	return id;
}

bool Event::operator > (const Event& other) const
{
	return (next > other.next);
}

Scheduler::Scheduler()
{
	next = 0;
	threadrunning = false;
	pthread_mutex_init(&queuelock, NULL);
	pthread_cond_init(&eventclock, NULL);
	StartEventLoop();
}

Scheduler::~Scheduler()
{
	StopEventLoop();
	pthread_mutex_destroy(&queuelock);
	pthread_cond_destroy(&eventclock);
}

event_t Scheduler::AddEvent(Event e)
{
	e.SetID(this);
	QueueLock();
	eventqueue.push(e);
	QueueUnlock();
	NotifyEventThread();
	return e.GetID();
}

void Scheduler::RemoveEvent(event_t id)
{
	QueueLock();
	cancelled.insert(id);
	QueueUnlock();
}

event_t Scheduler::NextAvailableID()
{
	return ++next;
}

void Scheduler::QueueLock()
{
	pthread_mutex_lock(&queuelock);
}

void Scheduler::QueueUnlock()
{
	pthread_mutex_unlock(&queuelock);
}

bool Scheduler::StartEventLoop()
{
	if (threadrunning) return false;
	threadrunning = true;
	pthread_create(&eventthread, NULL, Scheduler::DoRunEventLoop, (void *) this);
}

void Scheduler::StopEventLoop()
{
	if (!threadrunning) return;
	threadrunning = false;
	NotifyEventThread();
	pthread_join(eventthread, NULL);
	return;
}

/* Thread Operations */
void * Scheduler::DoRunEventLoop(void * arg)
{
	((Scheduler *) arg)->RunEventLoop();
	pthread_exit(NULL);
	return NULL;
}

void Scheduler::RunEventLoop()
{
	QueueLock();
	while (threadrunning)
	{
		SleepUntilNextEvent();

		while (!eventqueue.empty())
		{
			Event e = eventqueue.top();
			if (!e.Due()) break;
			eventqueue.pop();
			if (cancelled.find(e.GetID()) != cancelled.end())
			{
				cancelled.erase(e.GetID());
				e.Cancel();
				continue;
			}
			
			QueueUnlock();
			e.DoEvent();
			QueueLock();

			e.Next();
			if (e.ShouldReschedule()) eventqueue.push(e);
		}
	}
	QueueUnlock();
	return;
}

void Scheduler::SleepUntilNextEvent()
{
	if (eventqueue.empty())
	{
		pthread_cond_wait(&eventclock, &queuelock);
	}
	else
	{
		timespec t = Bottime::GetMillisAsTimespec(eventqueue.top().Countdown() + Bottime::GetCurrentTimeMillis());
		pthread_cond_timedwait(&eventclock, &queuelock, &t);
	}
}

/* Thread Support Functions */

void Scheduler::NotifyEventThread()
{
	pthread_cond_signal(&eventclock);
}
