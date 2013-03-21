#ifndef _MESSENGER_H_
#define _MESSENGER_H_

#include <string>

class IRCBot;

using namespace std;

class Messenger
{
	private:
	IRCBot& bot;
	string target;

	bool notmute;

	public:
	Messenger(IRCBot& bot, string target, bool mute);

	void Say(string message);
	void Message(string message);
	void Notice(string message);
	void Raw(string message);

	string GetTarget();
};

#endif
