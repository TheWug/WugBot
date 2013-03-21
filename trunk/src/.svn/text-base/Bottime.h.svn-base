#ifndef _BOTTIME_H_
#define _BOTTIME_H_

#include <ctime>

typedef long long mtime_t;

class Bottime
{
public:
	static mtime_t GetCurrentTimeMillis();
	static timespec GetCurrentTimeTimespec();

	static timespec GetMillisAsTimespec(mtime_t millis);
	static mtime_t GetTimespecAsMillis(timespec ts);

	static mtime_t GetUptime();
	static void SleepMillis(mtime_t millis);
private:
	static const mtime_t launchtime;
};

#endif
