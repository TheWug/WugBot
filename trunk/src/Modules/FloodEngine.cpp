#include <iostream>
#include <algorithm>

#include "FloodEngine.h"

#include "AntiFlood.h"
#include "FloodEvents.h"
#include "Scheduler.h"
#include "IRCBot.h"
#include "misc.h"

void FloodEngine::Analyze(FloodProtectModule * base, Hostname speaker, string target, Messenger respondto, IRCBot& bot)
{
	cout << "processing generic flood event in" << target << endl;
	return;
}

void FloodEngine::HandleNewEvent(FloodProtectModule * base, FloodType f, mtime_t timestamp, Hostname speaker, string target, string message, string repealmessage)
{
	string ltarget = tolower(target);
	ActionEvent e;
	e.type = f;
	e.timestamp = timestamp;
	base->userevents[speaker.GetNick()].AddPenalty(e);
	base->AddChannelEvent(ltarget, e);
	string docommand = base->settings[ltarget].reaction[f];
	string undocommand = base->settings[ltarget].cleanup[f];
	base->PerformCommand(docommand, speaker, target, message);
	base->PerformCommandFuture(base->settings[ltarget].timeout[f], undocommand, speaker, target, repealmessage);
}

mtime_t MessageFloodEngine::FigureOutDelta(TimeLimitedQueue<string>& msgs, ChannelSettings& settings)
{
	int threshold_m = settings.msg_threshold[F_MSG];

	if (msgs.Size() < 2) return settings.time_threshold[F_MSG] * 2000; // twice as long as time threshold, will never trigger action
	mtime_t delta = msgs.GetTime(0) - msgs.GetTime(1);

	for (int i = 1; i < threshold_m && i < msgs.Size(); ++i)
	{
		delta = delta / 3 + 2 * (msgs.GetTime(0) - msgs.GetTime(i)) / (i * 3);
	}
	return delta;
}

void MessageFloodEngine::Analyze(FloodProtectModule * base, Hostname speaker, string target, Messenger respondto, IRCBot& bot)
{
	string ltarget = tolower(target);
	TimeLimitedQueue<string>& msgs = base->userevents[speaker.GetNick()].message[ltarget]; // last few of this user's messages in this channel

	mtime_t delta = FigureOutDelta(msgs, base->settings[ltarget]); // delta time, in millis, of the last message

	if (delta <= 0) delta = 1; // delta is going to be a dividend, and should not be zero
	float threshold_t = base->settings[ltarget].time_threshold[F_MSG];
	int threshold_m   = base->settings[ltarget].msg_threshold[F_MSG];

	float modifier = 1, temp;
	if (msgs.Size() > 1 && msgs.Get(0) == msgs.Get(1))
	{
		if (msgs.Size() > 2 && msgs.Get(0) == msgs.Get(1))
			modifier *= 2.5; // weight triple duplicates a lot more heavily
		else
			modifier *= 1.5; // weight double duplicates more heavily
	}
	temp = CruiseControlQuotient(msgs.Get(0));
	if (temp > .3 && msgs.Get(0).length() > 6)
		modifier += temp; // if message is mostly caps, weight more heavily

	if (((float) delta) * threshold_m / modifier < threshold_t * 1000) // (floodvalue / modifier) will be the adjusted amount of time sending these messages should take more than, in milliseconds
	{
		deque<ActionEvent>& penalty = base->userevents[speaker.GetNick()].penalty;
		FloodType t;
		if (penalty.size() > 0 && (msgs.GetTime(0) - penalty[0].timestamp) < 120000)
			t = F_MSG2;
		else
			t = F_MSG;
		HandleNewEvent(base, t, msgs.GetTime(0), speaker, target, "Message Flood", "End of Message Flood");
	}
	return;
}

float MessageFloodEngine::CruiseControlQuotient(string s)
{
	int caps = 0;
	for (int i = 0; i < s.length(); ++i)
		caps += ((((int) s[i]) == toupper(s[i])) ? 1 : 0);
	return ((float) caps) / s.length();
}

mtime_t NickFloodEngine::FigureOutDelta(TimeLimitedQueue<Nothing>& msgs, ChannelSettings& settings)
{
	int threshold_m = settings.msg_threshold[F_NICK];

	if (msgs.Size() < 2) return settings.time_threshold[F_NICK] * 2000;
	mtime_t delta = msgs.GetTime(0) - msgs.GetTime(1);

	for (int i = 1; i < threshold_m && i < msgs.Size(); ++i)
	{
		delta = delta / 3 + 2 * (msgs.GetTime(0) - msgs.GetTime(i)) / (i * 3);
	}
	return delta;
}

void NickFloodEngine::Analyze(FloodProtectModule * base, Hostname speaker, string target, Messenger respondto, IRCBot& bot)
{
	string ltarget = tolower(target);
	TimeLimitedQueue<Nothing>& nicks = base->userevents[speaker.GetNick()].nick; // last few of this user's nick changes
	mtime_t delta = FigureOutDelta(nicks, base->settings[ltarget]);
	if (delta == 0) delta = 1;

	float threshold_msg = base->settings[ltarget].msg_threshold[F_NICK];
	float threshold_time = base->settings[ltarget].time_threshold[F_NICK];

	if (((float) delta) * threshold_msg < threshold_time * 1000)
	{
		deque<ActionEvent>& penalty = base->userevents[speaker.GetNick()].penalty;
		FloodType t;
		if (penalty.size() > 0 && (nicks.GetTime(0) - penalty[0].timestamp) < 120000)
			t = F_NICK2;
		else
			t = F_NICK;
		HandleNewEvent(base, t, nicks.GetTime(0), speaker, target, "Nick Flood", "End of Nick Flood");
	}
	return;
}

void PingFloodEngine::Analyze(FloodProtectModule * base, Hostname speaker, string target, Messenger respondto, IRCBot& bot)
{
	return;
}

mtime_t CycleFloodEngine::FigureOutDelta(TimeLimitedQueue<Nothing>& msgs, ChannelSettings& settings)
{
	int threshold_m = settings.msg_threshold[F_CYCLE];

	if (msgs.Size() < 2) return settings.time_threshold[F_CYCLE] * 2000;
	mtime_t delta = msgs.GetTime(0) - msgs.GetTime(1);

	for (int i = 1; i < threshold_m && i < msgs.Size(); ++i)
	{
		delta = delta / 3 + 2 * (msgs.GetTime(0) - msgs.GetTime(i)) / (i * 3);
	}
	return delta;
}

void CycleFloodEngine::Analyze(FloodProtectModule * base, Hostname speaker, string target, Messenger respondto, IRCBot& bot)
{
	string ltarget = tolower(target);
	TimeLimitedQueue<Nothing>& parts = base->userevents[speaker.GetNick()].leave[ltarget]; // last few of this user's parts
	if (parts.Size() < (UserInfoCollector::MAX_ENTRIES - 1)) return;
	mtime_t delta = FigureOutDelta(parts, base->settings[ltarget]);
	if (delta == 0) delta = 1;

	float threshold_m = base->settings[ltarget].msg_threshold[F_CYCLE];
	float threshold_t = base->settings[ltarget].msg_threshold[F_CYCLE];

	if (((float) delta) * threshold_m < threshold_t * 1000)
	{
		deque<ActionEvent>& penalty = base->userevents[speaker.GetNick()].penalty;
		FloodType t;
		if (penalty.size() > 0 && (parts.GetTime(0) - penalty[0].timestamp) < 120000)
			t = F_CYCLE2;
		else
			t = F_CYCLE;
		HandleNewEvent(base, t, parts.GetTime(0), speaker, target, "Cycle Flood", "End of Cycle Flood");
	}
	return;
}

void ChannelFloodEngine::Analyze(FloodProtectModule * base, Hostname speaker, string target, Messenger respondto, IRCBot& bot)
{
	string ltarget = tolower(target);
	deque<ActionEvent>& chanevents = base->actioneventsbychannel[ltarget]; // last few of this user's nick changes
	if (chanevents.size() < 5) return;
	mtime_t delta = (chanevents[0].timestamp - chanevents[4].timestamp) / 4; // average delta millis per message (less == floody)
	if (delta == 0) delta = 1;
	float threshold_m = base->settings[ltarget].msg_threshold[F_CHAN];
	float threshold_t = base->settings[ltarget].time_threshold[F_CHAN];

	if (((float) delta) * threshold_m < threshold_t * 1000) // (floodvalue / modifier) will be the adjusted amount of time sending these messages should take more than, in milliseconds
	{
		deque<ActionEvent>& penalty = base->userevents[speaker.GetNick()].penalty;
		FloodType t;
		if (((float) delta) * threshold_m < threshold_t * 500)
			t = F_CHAN2;
		else
			t = F_CHAN;
		HandleNewEvent(base, t, chanevents[0].timestamp, speaker, target, "Channel Defense Mode", "Channel Defense Standby");
	}
	return;
}

