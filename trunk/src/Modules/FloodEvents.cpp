#include <iostream>

#include "FloodEvents.h"

Nothing Nothing::nothing; // lol

UserInfoCollector::UserInfoCollector()
{
	Touch();
}

void UserInfoCollector::AddMsg(string channel, string statement)
{
	Touch();
	message[channel].Add(statement);
}

void UserInfoCollector::AddLeave(string channel)
{
	Touch();
	leave[channel].Add(Nothing::nothing);
}

void UserInfoCollector::AddNick()
{
	Touch();
	nick.Add(Nothing::nothing);
}

void UserInfoCollector::AddPenalty(ActionEvent e)
{
	Touch();
	penalty.push_front(e);
	if (penalty.size() > MAX_ENTRIES) penalty.pop_back();
}

void UserInfoCollector::Touch()
{
	lastupdate = Bottime::GetCurrentTimeMillis();
}
