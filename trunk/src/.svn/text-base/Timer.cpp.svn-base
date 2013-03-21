#include <ctime>
#include <unistd.h>
#include <iostream>

#include "Timer.h"

using namespace std;

Timer::Timer()
{
	clock_gettime(CLOCK_REALTIME, &lasttime);
	timer = 0;
	eventweight = 1000;
	interval = 0;
	maxweight = 8000;
}

Timer::Timer(float min_interval, float event_weight, float max_weight)
{
	clock_gettime(CLOCK_REALTIME, &lasttime);
	timer = 0;
	eventweight = event_weight * 1000;
	interval = min_interval * 1000;
	maxweight = max_weight * 1000;
}

void Timer::Sleep()
{
	unsigned int sleepfor = 0;
	timespec time = {0,0};
	clock_gettime(CLOCK_REALTIME, &time);
	unsigned int deltatime = (time.tv_sec - lasttime.tv_sec) * 1000 + (time.tv_nsec - lasttime.tv_nsec) / 1000000;
	unsigned int deltaevent = (time.tv_sec - lastevent.tv_sec) * 1000 + (time.tv_nsec - lastevent.tv_nsec) / 1000000;

	if (deltatime > maxweight + maxweight)
		deltatime = maxweight + maxweight;

	if (timer + eventweight < deltatime) timer = 0;
	else timer = timer + eventweight - deltatime;

	if (timer > maxweight)
	{
		unsigned int temp = timer - maxweight;
		sleepfor = (sleepfor > temp) ? sleepfor : temp;
	}

	unsigned int stalltime = (deltaevent > interval ? 0 : interval - deltaevent);
	if (sleepfor <= stalltime)
		sleepfor = stalltime;

	lasttime = time;

	if (sleepfor != 0)
		usleep(sleepfor * 1000);

	clock_gettime(CLOCK_REALTIME, &lastevent);
}

void Timer::ReZero()
{
	timer = 0;
	lasttime.tv_sec = 0;
	lasttime.tv_nsec = 0;
}

void Timer::SetMinInterval(float i)
{
	interval = i * 1000;
}

void Timer::SetEventWeight(float w)
{
	eventweight = w * 1000;
}

void Timer::SetMaxAllowedWeight(float a)
{
	maxweight = a * 1000;
}

float Timer::GetMinInterval()
{
	return interval / 1000.0;
}

float Timer::GetEventWeight()
{
	return eventweight / 1000.0;
}

float Timer::GetMaxAllowedWeight()
{
	return maxweight / 1000.0;
}
