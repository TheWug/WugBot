#ifndef _EVENT_ACTIONS_H_
#define _EVENT_ACTIONS_H_

#include "IRCBot.h"

class action
{
	public:
	virtual void DoEvent() = 0;
	virtual ~action();
};

class FunctionCallAction : public action
{
	public:
	FunctionCallAction(void * (*function)(void *), void * item);
	~FunctionCallAction();
	void DoEvent();

	private:
	void * (*fn)(void *);
	void * arg;
};

class VoidFunctionCallAction : public action
{
	public:
	VoidFunctionCallAction(void (*function)());
	~VoidFunctionCallAction();
	void DoEvent();

	private:
	void (*fn)();
};

class BotCommandAction : public action
{
public:
	BotCommandAction(IRCBot& bot, string cmdstring, Hostname speaker, string target);
	~BotCommandAction();

	void DoEvent();

private:
	IRCBot& bot;
	string cmd;
	Hostname speaker;
	string target;
};

#endif
