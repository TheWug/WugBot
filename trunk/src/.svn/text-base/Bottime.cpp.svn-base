#include "Bottime.h"
#include <unistd.h>

const mtime_t Bottime::launchtime = Bottime::GetCurrentTimeMillis();

mtime_t Bottime::GetCurrentTimeMillis()
{
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return GetTimespecAsMillis(time);
}

timespec Bottime::GetCurrentTimeTimespec()
{
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return time;
}

mtime_t Bottime::GetUptime()
{
	return GetCurrentTimeMillis() - launchtime;
}

timespec Bottime::GetMillisAsTimespec(mtime_t millis)
{
	timespec time;
	time.tv_sec = millis / 1000LL;
	time.tv_nsec = (int) ((millis % 1000LL) * 1000000LL);
	return time;
}

mtime_t Bottime::GetTimespecAsMillis(timespec time)
{
	return ((long long) time.tv_sec) * 1000LL + ((long long) time.tv_nsec) / 1000000LL;
}

void Bottime::SleepMillis(mtime_t millis)
{
	usleep(millis * 1000);
}
