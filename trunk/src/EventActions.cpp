#include "EventActions.h"

action::~action()
{
}

void action::DoEvent()
{
}

FunctionCallAction::FunctionCallAction(void * (*function)(void *), void * item) : arg(item), fn(function)
{
}

FunctionCallAction::~FunctionCallAction()
{
}

void FunctionCallAction::DoEvent()
{
	fn(arg);
}

VoidFunctionCallAction::VoidFunctionCallAction(void (*function)()) : fn(function)
{
}

VoidFunctionCallAction::~VoidFunctionCallAction()
{
}

void VoidFunctionCallAction::DoEvent()
{
	fn();
}

BotCommandAction::BotCommandAction(IRCBot& b, string cmdstring, Hostname speaker, string target) : bot(b)
{
	this->cmd = cmdstring;
	this->speaker = speaker;
	this->target = target;
}

BotCommandAction::~BotCommandAction()
{
}

void BotCommandAction::DoEvent()
{
	bot.PerformCommand(speaker, target, target, cmd);
}
