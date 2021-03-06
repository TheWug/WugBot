#include "CommonEvents.h"

/*
* timestamp is ignored for send, and for others it's the time the network processed the event
* args: 0 is the network key
	1 is the line to send/receive from the network, if there is one.
*/
string CommonEvents::NET_ALL			= "net::*";    // blanket event mask for all net events
string CommonEvents::NET_SEND			= "net::send"; // active: network sends data as this event is processed
string CommonEvents::NET_RECV			= "net::recv"; // passive: called whenever the network reads a string
string CommonEvents::NET_CONNECT		= "net::conn"; // active: called to initiate a connect
string CommonEvents::NET_DISCONNECT		= "net::dcon"; // active: called to initiate a disconnect
string CommonEvents::NET_RECONNECT		= "net::rcon"; // active: called to initiate a reconnect
string CommonEvents::NET_FCLOSE			= "net::fdie"; // active: called to immediately terminate connection
string CommonEvents::NET_ENDED			= "net::end";  // passive: generated when a network is udisconnected
string CommonEvents::NET_STARTED		= "net::start";// passive: generated when a network is connected
string CommonEvents::NET_CONNECTFAIL		= "net::cfail";// passive: generated when a connection attempt fails

/*
* ALL OF THE IRC_* MESSAGES ARE OF THE FOLLOWING FORMAT:
* timestamp is the time the message was received (ignored for send)
* args:	0 is the network key
	1 is the prefix (if one was included, otherwise this arg is empty)
	2+ all subsequent arguments are the data to be sent
	this data should follow the rules of IRC:
	* only the last should contain whitespace
	* a colon is automatically inserted before the last argument to indicate final-argument
	* remember that the prefix is not required for client -> server messages OR server -> client messages (though the server will tend to include it).
	* if a prefix is not specified, YOUR USER is the origin of the message.
	* if a prefix is not specified for an outgoing message, that's ok, the server will likely ignore it anyway and automatically prepend your prefix before broadcasting to other clients
	* a prefix is included in an irc message iff the message BEGINS in a colon (:).
		so, {"1", "Wug", "PRIVMSG", "#somechannel", "Hello, this is text."} would send ":Wug PRIVMSG #somechannel :Hello, this is text." to network "1".
*/
string CommonEvents::IRC_ALL			= "irc::*";       // blanket event mask for all irc events
string CommonEvents::IRC_GENERAL		= "irc::general"; // passive: generated for all irc messages
string CommonEvents::IRC_WELCOME		= "irc::1";       // passive: generated upon receiving a 1 numeric from the server (indicates registration complete)
string CommonEvents::IRC_OTHER			= "irc::other";   // passive: generated for any irc message not handled by one of the following
string CommonEvents::IRC_NIU			= "irc::niu";     // passive: generated when bot attempts to use a nickname that's in use
string CommonEvents::IRC_PING			= "irc::ping";    // passive: generated when bot receives a ping
string CommonEvents::IRC_READY			= "irc::ready";   // passive: generated when bot receives a ping

string CommonEvents::IRC_AUTHCHANGE		= "irc::auth";    // passive: generated upon receiving ACCOUNT change (advanced auth)
string CommonEvents::IRC_USER_ENTER		= "irc::join";    // passive: generated by any user joining a channel by any manner
string CommonEvents::IRC_USER_LEAVE		= "irc::part";    // passive: generated by any user leaving a channel by any manner
string CommonEvents::IRC_USER_KICK		= "irc::kick";    // passive: generated when a user kicks another user
string CommonEvents::IRC_USER_QUIT		= "irc::quit";    // passive: generated when a user quits the network
string CommonEvents::IRC_USER_NICK		= "irc::nick";    // passive: generated when a user's name changes for any reason
string CommonEvents::IRC_CHANNEL_MESSAGE	= "irc::cmsg";    // passive: generated when a PRIVMSG sent to a channel is received
string CommonEvents::IRC_CHANNEL_NOTICE		= "irc::cnote";   // passive: generated when a NOTICE sent to a channel is received
string CommonEvents::IRC_PRIVATE_MESSAGE	= "irc::pmsg";    // passive: generated when a PRIVMSG sent directly to the user is received
string CommonEvents::IRC_PRIVATE_NOTICE		= "irc::pnote";   // passive: generated when a NOTICE sent directly to the user is received
string CommonEvents::IRC_SEND			= "irc::send";    // active: called to generate an irc message and send it to the network

/*
* properties manager events
* timestamp is the time the message was generated
* args for PROP_LOAD and PROP_SAVE: 0 is the filename.  blank or no args means default file is used.
* all others have no args.
*/
string CommonEvents::PROP_ALL			= "prp::*";       // blanket event mask for all properties events
string CommonEvents::PROP_READY			= "prp::ready";   // passive: emitted by the properties manager when it finishes loading (services that depend on properties) may now initialize
string CommonEvents::PROP_LOAD			= "prp::load";    // active: prompts the properties manager to load properties from disk
string CommonEvents::PROP_LOADED		= "prp::loaded";  // passive: generated when the properties manager has finished reloading properties from disk
string CommonEvents::PROP_SAVE			= "prp::save";    // active: prompts the properties manager to write all state to disk
string CommonEvents::PROP_SAVED			= "prp::saved";   // passive: generated when the properties manager has finished writing properties to disk
string CommonEvents::PROP_EDIT			= "prp::edit";    // passive: generated when something changes a property
string CommonEvents::PROP_AGGREGATE		= "prp::agg";    // passive: generated when something changes a property

/*
* These events indicate important events that all modules should listen for
* timestamp is the time the message was produced.
* args is always blank.
*/
string CommonEvents::BOT_ALL			= "bot::*";       // blanket event mask for all bot events
string CommonEvents::BOT_STARTUP		= "bot::start";   // passive: called immediately after the bot starts
string CommonEvents::BOT_SAVEALL		= "bot::save";    // active: initiates the bot force saving process
string CommonEvents::BOT_LOADALL		= "bot::load";    // active: initiates the bot force loading process
string CommonEvents::BOT_DIE			= "bot::die";     // active: causes the bot to begin the shutdown process
string CommonEvents::BOT_SHUTDOWN		= "bot::stop";    // passive: generated to indicate the bot is shutting down
string CommonEvents::BOT_FIN			= "bot::fin";     // passive: last stage of bot shutdown. no system should generate events in response to this as they may never be executed.
string CommonEvents::BOT_TERMINATE		= "bot::term";    // active: the bot core, upon receiving this, will tear down bot state and return control to the main thread.
string CommonEvents::BOT_SIGNAL			= "bot::sig";     // passive: called when the process receives a catchable signal

/*
* module manager events
* timestamp is the time the message was produced
* args:	0 is always module name
*/
string CommonEvents::MOD_ALL			= "mod::*";       // blanket event mask for all mod events
string CommonEvents::MOD_LOAD			= "mod::load";    // active: instructs the module handler to load a module
string CommonEvents::MOD_INIT			= "mod::init";    // passive: distributed by the module loader to indicate the module is now alive
string CommonEvents::MOD_HELLO			= "mod::hello";   // passive: sent by module that has just self-initialized
string CommonEvents::MOD_UNLOAD			= "mod::unload";  // active: instructs the module handler to unload module
string CommonEvents::MOD_FIN			= "mod::fin";     // passive: distributed by the module loader to indicate module is being unloaded
string CommonEvents::MOD_GOODBYE		= "mod::bye";     // passive: last message sent by dying module

