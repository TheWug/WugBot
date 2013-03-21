#include "BotTime.h"
#include <unistd.h>

const mtime_t BotTime::launchtime = BotTime::GetCurrentTimeMillis();

mtime_t BotTime::GetCurrentTimeMillis()
{
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return GetTimespecAsMillis(time);
}

timespec BotTime::GetCurrentTimeTimespec()
{
	timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return time;
}

mtime_t BotTime::GetUptime()
{
	return GetCurrentTimeMillis() - launchtime;
}

timespec BotTime::GetMillisAsTimespec(mtime_t millis)
{
	timespec time;
	time.tv_sec = millis / 1000LL;
	time.tv_nsec = (int) ((millis % 1000LL) * 1000000LL);
	return time;
}

mtime_t BotTime::GetTimespecAsMillis(timespec time)
{
	return ((long long) time.tv_sec) * 1000LL + ((long long) time.tv_nsec) / 1000000LL;
}

void BotTime::SleepMillis(mtime_t millis)
{
	usleep(millis * 1000);
}
