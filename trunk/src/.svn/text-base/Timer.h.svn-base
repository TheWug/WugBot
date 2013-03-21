#ifndef _TIMER_H_
#define _TIMER_H_

#include <ctime>
#include <iostream>
#include <unistd.h>

class Timer
{
	public:
	Timer();
	Timer(float min_interval, float event_weight, float max_weight);

	void Sleep();
	void ReZero();

	void SetMinInterval(float i);
	void SetEventWeight(float w);
	void SetMaxAllowedWeight(float a);

	float GetMinInterval();
	float GetEventWeight();
	float GetMaxAllowedWeight();

	private:
	unsigned int timer;
	timespec lasttime;
	timespec lastevent;

	int eventweight;
	int interval;
	int maxweight;
};

#endif
