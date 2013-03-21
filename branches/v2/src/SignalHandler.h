#ifndef _SIGNAL_HANDLER_H_
#define _SIGNAL_HANDLER_H_

#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <string>

using namespace std;

class SignalHandler
{
public:
	typedef void (*SigHandler)(siginfo_t& info);
private:
	bool running;
	pthread_t thread;
	SigHandler handler;

	static void * DoSignalHandlingLoop(void * arg);
	void SignalHandlingLoop();
public:
	SignalHandler(SigHandler h);

	void Setup();
	void Engage();
	void Disengage();

	static string GetSignalName(int signal);
};

#endif
