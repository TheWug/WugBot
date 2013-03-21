#ifndef _FLOODEVENTS_H_
#define _FLOODEVENTS_H_

#include <string>
#include <deque>
#include <map>

#include "Bottime.h"

using namespace std;

enum FloodType
{
	F_CYCLE = 0,
	F_MSG = 1,
	F_PING = 2,
	F_NICK = 3,
	F_CHAN = 4,
	F_CYCLE2 = 5,
	F_MSG2 = 6,
	F_PING2 = 7,
	F_NICK2 = 8,
	F_CHAN2 = 9
};

struct Nothing
{
public:
	static Nothing nothing;
};

struct ActionEvent
{
public:
	mtime_t timestamp;
	FloodType type;
};

struct TimedOccurrence
{
	string occurrence;
	mtime_t order;
};

template <typename T>
class TimeLimitedQueue
{
public:
	TimeLimitedQueue();

	void Reset(mtime_t maxage, int maxsize);
	void Add(const T& n);
	T& Get(int n);
	mtime_t GetTime(int n);
	void Poke();
	int Size();
	int MaxSize();

	struct Entry
	{
	public:
		mtime_t time;
		T object;
	};

private:
	int maxsize;
	mtime_t maxage;
	deque<Entry> queue;
};

template <typename T> TimeLimitedQueue<T>::TimeLimitedQueue()
{
	this->maxage = 60000;
	this->maxsize = 10;
}

template <typename T> void TimeLimitedQueue<T>::Reset(mtime_t maxage, int maxsize)
{
	this->maxage = maxage;
	this->maxsize = maxsize;
	Poke();
}

template <typename T> void TimeLimitedQueue<T>::Add(const T& n)
{
	Entry e;
	e.time = Bottime::GetCurrentTimeMillis();
	e.object = n;
	queue.push_front(e);
	Poke();
}

template <typename T> T& TimeLimitedQueue<T>::Get(int n)
{
	return queue[n].object;
}

template <typename T> mtime_t TimeLimitedQueue<T>::GetTime(int n)
{
	return queue[n].time;
}

template <typename T> int TimeLimitedQueue<T>::Size()
{
	return queue.size();
}

template <typename T> int TimeLimitedQueue<T>::MaxSize()
{
	return maxsize;
}

template <typename T> void TimeLimitedQueue<T>::Poke()
{
	mtime_t now = Bottime::GetCurrentTimeMillis();
	if (queue.size() > maxsize) queue.pop_back();
	while(queue.size() > 0 && (now - queue.back().time) > maxage) queue.pop_back();
}

class UserInfoCollector
{
public:
	UserInfoCollector();

	void AddMsg(string channel, string statement);
	void AddLeave(string channel);
	void AddNick();
	void Touch();

	void AddPenalty(ActionEvent e);

	TimeLimitedQueue<Nothing> nick;
	map<string, TimeLimitedQueue<Nothing> > leave;
	map<string, TimeLimitedQueue<string> > message;

	deque<ActionEvent> penalty;

	mtime_t lastupdate;
	static const int MAX_ENTRIES = 4;
};

#endif
