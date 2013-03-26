#include <unistd.h>
#include <iostream>

#include "BotCore.h"
#include "SignalHandler.h"
#include "BotTime.h"
#include "StringUtil.h"
#include "CommonEvents.h"
#include "Scheduler.h"

static BotCore * core;

void SigHandlerFunction(siginfo_t& info)
{
	if (core == NULL) cout << "Got a signal even before initialization was done!  you broke it good." << endl;
	else 
	{
		EventHandler::Event e;
		e.name = CommonEvents::BOT_SIGNAL;
		e.data.push_back(StringUtil::FromInt32(info.si_signo));
		e.data.push_back(SignalHandler::GetSignalName(info.si_signo));
		core->BotEvents().Raise(e);
	}
	return;
}

/*

void * EventTestMain(void * x)
{
	EventHandler::Event dumbevent;
	
	dumbevent.name = "LOLOL::ROFL";
	for (int i = 0; i < 25; ++i)
	{
		core->BotEvents().Raise(dumbevent);
	}
	return NULL;
}

void * OccurrenceTestMain(void * x)
{
	EventHandler::Event dumbevent;
	dumbevent.name = "test::chirp";
	dumbevent.data.push_back("");
	Scheduler::Occurrence dumboccr(0, -1, dumbevent);

	Scheduler::occur_t ids[25];
	
	for (int i = 0; i < 25; ++i)
	{
		dumboccr.event.data[0] = StringUtil::FromInt32(i);
		dumboccr.interval = 100 * (i + 1);
		ids[i] = core->BotSched().AddOccurrence(dumboccr);
	}
	sleep(10);
	for (int i = 0; i < 25; ++i) core->BotSched().RemoveOccurrence(ids[i]);
	sleep(1);
	cout << "removed all occurrences" << endl;
	sleep(5);
	return NULL;
}

void EventTest()
{
	pthread_t threads[8];
	for (int i = 0; i < 8; ++i) pthread_create(threads + i, NULL, EventTestMain, NULL);
	for (int i = 0; i < 8; ++i) pthread_join(threads[i], NULL);
	sleep(10);
	return;
}

void OccurrenceTest()
{
	pthread_t threads[8];
	for (int i = 0; i < 8; ++i) pthread_create(threads + i, NULL, OccurrenceTestMain, NULL);
	for (int i = 0; i < 8; ++i) pthread_join(threads[i], NULL);
	return;
} */

int main()
{
	core = NULL;
	SignalHandler sh(SigHandlerFunction);
	sh.Setup();
	sh.Engage();

	BotCore bot;
	core = &bot;

	bot.StartBot(true, "ircbot.conf"); // true -> block until bot quits
					    // false -> don't block

	//cout << "testing soon..." << endl;
	//sleep(1);
	//OccurrenceTest();
	//cout << "still alive!" << endl;

	//bot.StopBot("test over");

	sh.Disengage();
}
