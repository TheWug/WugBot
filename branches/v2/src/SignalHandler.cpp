#include "SignalHandler.h"

SignalHandler::SignalHandler(SigHandler h)
{
	running = false;
	handler = h;
}

void SignalHandler::Setup()
{
	sigset_t s;
	sigfillset(&s);
	pthread_sigmask(SIG_BLOCK, &s, NULL);
}

void * SignalHandler::DoSignalHandlingLoop(void * arg)
{
	SignalHandler * self = (SignalHandler *) arg;
	self->running = true;
	self->SignalHandlingLoop();
	return NULL;
}

void SignalHandler::SignalHandlingLoop()
{
	sigset_t s;
	sigfillset(&s);
	siginfo_t info;
	int i = sigwaitinfo(&s, &info);
	while (running)
	{
		if (i >= 0) handler(info);
		i = sigwaitinfo(&s, &info);
	}
}

void SignalHandler::Engage()
{
	pthread_create(&thread, NULL, DoSignalHandlingLoop, this);
}

void SignalHandler::Disengage()
{
	running = false;
	pthread_kill(thread, SIGUSR1); // wake up thread with useless signal
	pthread_join(thread, NULL);
}

string SignalHandler::GetSignalName(int signal)
{
	const char * str = strsignal(signal);
	if (str == NULL) str = "Unknown Signal";
	return str;
}
