#ifndef _COMMON_EVENTS_H_
#define _COMMON_EVENTS_H_

#include <string>

using namespace std;

class CommonEvents
{
public:
	static string NET_SEND;
	static string NET_RECV;
	static string NET_CONNECT;
	static string NET_DISCONNECT;
	static string NET_RECONNECT;
	static string NET_FCLOSE;
	static string NET_CONNLOST;
	static string NET_ENDED;

	static string IRC_GENERAL;
	static string IRC_OTHER;
	static string IRC_USER_ENTER;
	static string IRC_USER_LEAVE;
	static string IRC_USER_KICK;
	static string IRC_USER_QUIT;
	static string IRC_USER_NICK;
	static string IRC_CHANNEL_MESSAGE;
	static string IRC_CHANNEL_NOTICE;
	static string IRC_PRIVATE_MESSAGE;
	static string IRC_PRIVATE_NOTICE;
	static string IRC_SEND;

	static string PROP_LOAD;
	static string PROP_LOADED;
	static string PROP_SAVE;
	static string PROP_SAVED;

	static string BOT_STARTUP;
	static string BOT_SAVEALL;
	static string BOT_LOADALL;
	static string BOT_DIE;
	static string BOT_SHUTDOWN;
	static string BOT_SIGNAL;
	static string BOT_TERMINATED;

	static string MOD_LOAD;
	static string MOD_INIT;
	static string MOD_HELLO;
	static string MOD_UNLOAD;
	static string MOD_FIN;
	static string MOD_GOODBYE;
};

#endif
