#include <unistd.h>
#include <iostream>

#include "BotCore.h"
#include "SignalHandler.h"
#include "BotTime.h"
#include "StringUtil.h"
#include "CommonEvents.h"

static BotCore * core;

void H(siginfo_t& info)
{
	if (core == NULL) cout << "Got a signal even before initialization was done!  you broke it good." << endl;
	else 
	{
		core->BotLog().GetLog(BotLogger::SYS).Put(WARNING, "Received signal: " + SignalHandler::GetSignalName(info.si_signo));
		EventHandler::Event e;
		e.name = CommonEvents::BOT_SIGNAL;
		e.timestamp = BotTime::GetCurrentTimeMillis();
		e.data.push_back(StringUtil::FromInt32(info.si_signo));
		core->BotEvents().Raise(e);
	}
}

int main()
{
	core = NULL;
	SignalHandler sh(H);
	sh.Setup();
	sh.Engage();

	BotCore bot;
	core = &bot;

	bot.StartBot(true, "ircbot.conf"); // true -> block until bot quits

	sh.Disengage();
}
