#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <unistd.h>

#include "IRCBot.h"
#include "BotInfo.h"
#include "BotPermissions.h"
#include "UserManager.h"
#include "ModuleHandler.h"
#include "misc.h"
#include "Hostname.h"
#include "Message.h"
#include "DCmd.h"
#include "IRC.h"
#include "Cmd.h"
#include "Rsp.h"
#include "errors.h"
#include "ServerProperties.h"
#include "Timer.h"
#include "Bottime.h"
#include "Scheduler.h"

using namespace std;

IRCBot::IRCBot(BotInfo& b) : 
				permission(*(new BotPermissions())),
				usermgr(*(new UserManager())), 
				serverproperties(*(new ServerProperties())), 
				network(*(new IRCBotNetIface())), 
				modhandler(*(new ModuleHandler(*this))),
				scheduler(*(new Scheduler()))
{
	nick = b.GetNick();
	altnick = b.GetAltNick();
	lnick = tolower(nick);
	nsaccount = b.GetAccount();
	nspassword = b.GetPass();
	realname = b.GetRealname();
	user = b.GetUser();
	logtarget = b.GetLogChannel();

	permission.SetOwner(tolower(b.GetOwner()));
	connectcommands = vector<string>();
	commandprefix = b.GetCommandPrefix().at(0);

	authenticated = false;
	connected = false;
	shuttingdown = false;
	registered = false;

	readonly = b.GetReadOnly();
	silent = false;

 	(new CmdJoin())->Register(*this);
	(new CmdPart())->Register(*this);
	(new CmdSay())->Register(*this);
	(new CmdTell())->Register(*this);
	(new CmdQuit())->Register(*this);
	(new CmdAllow())->Register(*this);
	(new CmdNick())->Register(*this);
	(new CmdFwd())->Register(*this);
	(new CmdPoke())->Register(*this);
	(new CmdOnUser(*this))->Register(*this);
	(new CmdAccess())->Register(*this);
	(new CmdKick())->Register(*this);
	(new CmdIP())->Register(*this);
	(new CmdFactoid(*this))->Register(*this);
	(new CmdHelp())->Register(*this);
	(new CmdRaw())->Register(*this);
	(new CmdOp())->Register(*this);
	(new CmdVoice())->Register(*this);
	(new CmdBan())->Register(*this);
	(new CmdQuiet())->Register(*this);
	(new CmdExempt())->Register(*this);
	(new CmdMode())->Register(*this);
	(new CmdQueue())->Register(*this);
	(new CmdOn(*this))->Register(*this);
	(new CmdTopic())->Register(*this);
	(new CmdAlias())->Register(*this);
	(new CmdDie())->Register(*this);
	(new CmdFlags())->Register(*this);
	(new CmdPrefix())->Register(*this);
	(new CmdCtcpResp(*this))->Register(*this);
	(new CmdSave())->Register(*this);
	(new CmdMemo(*this))->Register(*this);
	(new CmdWhere())->Register(*this);
	(new CmdCheck())->Register(*this);
	(new CmdDNS())->Register(*this);
	(new CmdModule())->Register(*this);
	(new CmdLong())->Register(*this);
	(new CmdMonitor(*this))->Register(*this);
	(new CmdHaspaid())->Register(*this);
	(new CmdTime())->Register(*this);
	(new CmdOption())->Register(*this);
	(new CmdRepeat())->Register(*this);

	(new RspPrivmsg())->Register(*this);
	(new RspNotice())->Register(*this);
	(new RspJoin())->Register(*this);
	(new RspPart())->Register(*this);
	(new RspQuit())->Register(*this);
	(new RspNick())->Register(*this);
	(new RspNickInUse())->Register(*this);
	(new RspKick())->Register(*this);
	(new RspMOTD())->Register(*this);
	(new RspAccount())->Register(*this);
	(new RspConnect())->Register(*this);
	(new RspInvite())->Register(*this);

	permission.ReadState();
	usermgr.ReadState();

	ReadConnectCommands(b.GetConnectCommands());
	Timer& floodlimiter = network.GetTimer();
	floodlimiter.SetMinInterval(b.GetFloodMinInterval());
	floodlimiter.SetEventWeight(b.GetFloodEventWeight());
	floodlimiter.SetMaxAllowedWeight(b.GetFloodMaxWeight());
}

IRCBot::~IRCBot()
{
	if (readonly) cout << "Read only mode is set, discarding all configuration changes..." << endl;
	FinishConnect();
	CleanupCommands();
	CleanupModules();
	for (map<string, rsp *>::iterator i = responses.begin(); i != responses.end(); i++)
	{
		if ((*i).second != 0)
			delete (*i).second;
	}
	if (!readonly)
	{
		cout << "Saved state for all commands.\nSaving permissions..." << endl;
		permission.SaveState();
		cout << "Saving usermanagers..." << endl;
		usermgr.SaveState();
		cout << "State completely saved." << endl;
	}
	delete &permission;
	delete &usermgr;
	delete &serverproperties;
}

void IRCBot::ReadConnectCommands(string ccfilename)
{
	string line;
	ifstream ccfile(ccfilename.c_str(), ios::in);
	if (ccfile.is_open())
	{
		getline(ccfile, line);
		while (ccfile.good())
		{
			connectcommands.push_back(line);
			getline(ccfile, line);
		}
		ccfile.close();
	}
	else
		cout << "Error reading connect command file: " + ccfilename + " (perhaps it does not exist?)" << endl;
}

void IRCBot::InitialConnect(string server, int port)
{
	shuttingdown = false;
	network.BindServer(server, port);
	connected = (network.Connect() == M_SUCCESS);
	if (!connected)
	{
		cout << "Connection to IRC server " << server << " could not be established, giving up..." << endl;
		return;
	}

	int i = 0;
	char * readbuffer = 0;
	readbuffer = network.Receive();
	if (readbuffer != 0)
	{
		Process(readbuffer);
	}
	else
	{
		cout << "Error with input.  Terminating..." << endl;
		return;
	}
	Register();
}

void IRCBot::ProcessLoop()
{
	char * readbuffer = 0;
	while (!shuttingdown)
	{
		readbuffer = network.Receive();
		if (readbuffer != 0)
		{
			Process(readbuffer);
		}
		else
		{
			Reconnect();
			servername = "";
		}
	}
	cout << "returning: " << shuttingdown << endl;
}

void IRCBot::FinishConnect()
{
	if (connected)
	{
		network.Disconnect();
		connected = false;
		servername = "";
	}
}

void IRCBot::Reconnect()
{
	registered = false;
	authenticated = false;
	network.Disconnect();
	connected = (network.Connect() == M_SUCCESS);

	if (!connected)
	{
		cout << "Connection could not be reestablished, giving up..." << endl;
		return;
	}

	char * readbuffer = network.Receive();
	if (readbuffer != 0)
		Process(readbuffer);
	else
	{
		FinishConnect();
		cout << "Error with input.  Terminating..." << endl;
		return;
	}
	Register();
}

void IRCBot::Process(string response)
{
	vector<string> elems = splitcommands(response, "\r\n", false);
	string token;

	for (int i = 0; i < elems.size(); i++)
	{
		istringstream line(elems.at(i));
		line >> token;
		token = tolower(token);
		if (token == "ping")
		{
			Pong(elems.at(i).substr(token.length() + 1, elems.at(i).length() - token.length() - 1));
			return;
		}
		else if (token == "error")
		{
			connected = false;
			return;
		}
		else if (token.at(0) == ':')
		{
			ProcessFrom(elems.at(i).substr(1, token.length() - 1), getline(line));
		}
	}
}

void IRCBot::ProcessFrom(string origin, string response)
{
	Hostname h(origin);

	istringstream resp(response);
	string command;
	resp >> command;
	command = tolower(command);

	if (command == "001")
		servername = h.GetNickL();

	string account = usermgr.GetUsernameFromNick(h.GetNick());

	if ((account == "") && (command == "join") && (h.GetNickL() != servername) && (h.GetNickL().find('.') == -1))
	{
		vector<string> blocks = splitcommands(response, " ", false);
		if (blocks.size() >= 4)
			account = tolower(blocks[2]);
	}
	else if ((account == "") && (h.GetNickL() != servername) && h.GetNickL().find('.') == -1)
	{
		AddDeferredCommand(new DeferredAccessCheck(*this, command, h, getline(resp)));
		return;
	}
	h.SetAccount(account);
	ProcessFromCmd(h, command, getline(resp));
}

void IRCBot::ProcessFromCmd(Hostname& origin, string& command, string response)
{
	istringstream resp(response);
	string target;
	resp >> target;
	bool ctcpflag = false;
	response = getline(resp);

	if (response.size() != 0 && response[0] == ':')
		response = response.substr(1, response.size() - 1);
	if (target.size() != 0 && target[0] == ':')
		target = target.substr(1, target.size() - 1);

	if (response.size() != 0 && response[0] == 1)
	{
		response = response.substr(1, response.size() - 2);
		ctcpflag = true;
	}

	ProcessInputFirst(origin, command, target, response, ctcpflag);
	ProcessModule(origin, command, target, response, ctcpflag);
	ProcessDeferred(origin, command, target, response, ctcpflag);
	ProcessInputSecond(origin, command, target, response, ctcpflag);
}

void IRCBot::ProcessInputFirst(Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	rsp * response = responses[command];
	if (response != 0)
	{
		response->PreAction(*this, speaker, command, target, message, ctcpflag);
	}
	return;
}

void IRCBot::ProcessInputSecond(Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	rsp * response = responses[command];
	if (response != 0)
	{
		response->PostAction(*this, speaker, command, target, message, ctcpflag);
	}
	return;
}

void IRCBot::ProcessModule(Hostname& speaker, string& command, string target, string message, bool ctcpflag)
{
	modhandler.UpdateModules(speaker, command, target, message, ctcpflag);
}

bool IRCBot::ProcessDeferred(Hostname speaker, string command, string target, string message, bool ctcpflag)
{
	int length = deferred.size();
	bool returnvalue = false;
	set<dcmd *> expired;
	for (set<dcmd *>::iterator i = deferred.begin(); i != deferred.end(); ++i)
	{
		if ((*i)->Call(command, speaker, target, GetRespondTo(speaker, target), message, ctcpflag) == true)
			expired.insert(*i);
	}
	returnvalue = (expired.size() != 0);
	for (set<dcmd *>::iterator i = expired.begin(); i != expired.end(); ++i)
	{
		delete (*i);
		deferred.erase(*i);
	}
	return returnvalue;
}

bool IRCBot::ParseCommand(Hostname& speaker, string target, string respondto, string message)
{
	istringstream gc(message);
	string command;
	gc >> command;
	command = tolower(command);
	if (command.length() > 0 && (command.at(0) < 97 && command.at(0) > 122 && command.size() > 0))
		command = command.substr(1, command.size() - 1);
	if ((speaker.GetNickL() != lnick) && permission.CanUseCommand(speaker.GetAccountL(), command))
		return DoCommand(command, speaker, target, respondto, getline(gc));
	else if (TakesFlags(command))
		return DoCommand(command, speaker, target, respondto, getline(gc), true);
	else
		cout << speaker.GetNick() << " tried to use command: " << command << endl;
	return false;
}

bool IRCBot::PerformCommand(Hostname& speaker, string target, string respondto, string message)
{
	istringstream gc(message);
	string command;
	gc >> command;
	command = tolower(command);
	if (command.length() > 0 && (command.at(0) < 97 && command.at(0) > 122 && command.size() > 0))
		command = command.substr(1, command.size() - 1);
	return DoCommand(command, speaker, target, respondto, getline(gc));
}

bool IRCBot::DoCommand(string& command, Hostname& speaker, string target, string respondto, string input, bool takesflags)
{
	map<string, cmd *>::iterator c = commands.find(command);
	if (c == commands.end())
	{
		cout << command + ": Command Not Found" << endl;
		return false;
	}
	cmd * found = c->second;
	if (takesflags)
		found->ProcessCommandFlags(*this, command, speaker, target, respondto, input);
	else
		found->ProcessCommand(*this, command, speaker, target, respondto, input);
	return true;
}

bool IRCBot::TakesFlags(string command)
{
	map<string, cmd *>::iterator c = commands.find(command);
	if (c == commands.end())
		return false;
	return c->second->AcceptsFlags();
}

void IRCBot::ChatResponse(Hostname& speaker, string target, string, string& message)
{
}

string IRCBot::GetCTCPResponse(string ctcpcommand, Hostname speaker, string ctcpargs)
{
	map<string, string>::iterator i = ctcpresponses.find(ctcpcommand);
	if (i == ctcpresponses.end())
		return "";
	string s = stringreplace(i->second, "%n", speaker.GetNick());
	s = stringreplace(s, "%N", nick);
	s = stringreplace(s, "%S", servername);
	s = stringreplace(s, "%T", AsHumanReadableTime(Bottime::GetUptime()));
//	s = stringreplace(s, "%t", current_system_time);
	s = stringreplace(s, "%a", ctcpargs);
	return s;
}

void IRCBot::ProcessCTCP(Hostname& speaker, string& command, string target, string& ctcpcommand, string ctcpargs)
{
	string s = GetCTCPResponse(ctcpcommand, speaker, ctcpargs);
	if (s != "")
		CTCPReply(speaker.GetNick(), s);
}

void IRCBot::CTCPReply(string target, string& message)
{
	network.Send(IRC::Notice(target, "\001" + message + "\001"));
	return;
}

void IRCBot::NSAuth()
{
	cout << "Authenticating with nickserv..." << endl;
	authenticated = true;
	network.Send(IRC::Message("nickserv", "identify " + nsaccount + " " + nspassword));
}

void IRCBot::Pong(string time)
{
	if (time.at(0) == ':')
		network.Send("PONG " + time + "\r\n");
}

void IRCBot::Quit(string reason)
{
	network.Send(IRC::Quit(splitandterminate(reason)[0]));
	cout << "quitting..." << endl;
	shuttingdown = true;
}

void IRCBot::Join(string channelname)
{
	network.Send(IRC::Join(splitandterminate(channelname)[0]));
}

void IRCBot::Part(string channelname, string reason)
{
	network.Send(IRC::Part(splitandterminate(channelname)[0], splitandterminate(reason)[0]));
}

void IRCBot::Say(string target, string message)
{
	vector<string> s = splitandterminate(message);
	for (vector<string>::iterator i = s.begin(); i != s.end(); i++)
	{
		if (target != "")
			network.Send(IRC::Message(target, *i));
		else
			cout << *i << endl;
	}
}

void IRCBot::Message(string target, string message)
{
	vector<string> s = splitandterminate(message);
	for (vector<string>::iterator i = s.begin(); i != s.end(); i++)
	{
		if (target != "")
			network.Send(IRC::Message(target, *i));
		else
			cout << *i << endl;
	}
}

void IRCBot::Notice(string target, string message)
{
	vector<string> s = splitandterminate(message);
	for (vector<string>::iterator i = s.begin(); i != s.end(); i++)
	{
		if (target != "")
			network.Send(IRC::Notice(target, *i));
		else
			cout << *i << endl;
	}
}

void IRCBot::Action(string target, string message)
{
	vector<string> s = splitandterminate(message);
	for (vector<string>::iterator i = s.begin(); i != s.end(); i++)
	{
		if (target != "")
			network.Send(IRC::Message(target, string("\x01") + "ACTION " + *i + "\x01"));
		else
			cout << nick + " " + *i << endl;
	}
		
}

void IRCBot::Raw(string data)
{
	network.Send(data + "\r\n");
}

void IRCBot::Nick(string newnick)
{
	network.Send(IRC::Nick(splitandterminate(newnick)[0]));
}

void IRCBot::Kick(string nick, string channel, string reason)
{
	network.Send(IRC::Kick(splitandterminate(nick)[0], splitandterminate(channel)[0], splitandterminate(reason)[0]));
}

void IRCBot::Mode(string target, string mode)
{
	network.Send(IRC::Mode(splitandterminate(target)[0], splitandterminate(mode)[0]));
}

void IRCBot::Whois(string nick)
{
	network.Send(IRC::Whois(splitandterminate(nick)[0]));
}

void IRCBot::Who(string channel, string arguments)
{
	network.Send(IRC::Who(splitandterminate(channel)[0], splitandterminate(arguments)[0]));
}

void IRCBot::Monitor(string action, string target)
{
	network.Send(IRC::Monitor(splitandterminate(action)[0], splitandterminate(target)[0]));
}

void IRCBot::Log(string data)
{
	Message(logtarget, data);
}

void IRCBot::AddCommand(string id, cmd * newcommand, int userlevel, string userflags)
{
	if (id == "" || userlevel < -1)
		return;
	commands[id] = newcommand;
	permission.SetDefaultCommandLevel(id, userlevel);
	permission.SetCommandFlags(id, userflags);
	registeredcommands.insert(newcommand);
	cout << "Command Registered: " + id << endl;
}

void IRCBot::RemoveCommand(string id)
{
	cmd * toremove = commands[id];
	commands.erase(id);
	if (toremove == 0)
		return;
	bool remains = false;
	for (map<string, cmd *>::iterator i = commands.begin(); i != commands.end(); i++)
	{
		if ((*i).second == toremove && (*i).second != 0)
		{
			remains = true;
			continue;
		}
	}
	if (!remains)
		registeredcommands.erase(toremove);
}

void IRCBot::CleanupCommands()
{
	if (!readonly)
	{
		for (set<cmd *>::iterator i = registeredcommands.begin(); i != registeredcommands.end(); i++)
		{
			cout << "Saving state of command: " << (*i) << " ..." << endl;
			(**i).SaveState();
			cout << (*i) << " Saved." << endl;
		}
	}
	for (set<cmd *>::iterator i = registeredcommands.begin(); i != registeredcommands.end(); i++)
		delete (*i);
}

void IRCBot::CleanupDeferredCommands()
{
	for (set<dcmd *>::iterator i = deferred.begin(); i != deferred.end(); i++)
		delete (*i);
}

void IRCBot::CleanupModules()
{
	modhandler.CleanupModules();
}

void IRCBot::SetResponse(string respondto, rsp * response)
{
	if (responses[respondto] != 0)
		delete responses[respondto];
	responses[respondto] = response;
}

void IRCBot::AddDeferredCommand(dcmd * d)
{
	if (d != NULL)
		deferred.insert(d);
}

void IRCBot::RemoveDeferredCommand(dcmd * d)
{
	deferred.erase(d);
}

void IRCBot::WhoChannel(string channel)
{
	AddDeferredCommand(new WhoTracker(channel, (*this)));
}

void IRCBot::Topic(string channel, string newtopic)
{
	network.Send(IRC::Topic(splitandterminate(channel)[0], splitandterminate(newtopic)[0]));
}

void IRCBot::Register()
{
	network.Send("NICK " + nick + "\r\nUSER " + user + " bot bot :" + realname + "\r\n");
}

string IRCBot::GetRespondTo(Hostname speaker, string target)
{
	if (target.length() != 0 && target.at(0) != '#')
		return speaker.GetNick();
	if (permission.CanSendTo(target))
		return target;
	else
		return speaker.GetNick();
}

void IRCBot::OnChannelEnter(string channel)
{
	WhoChannel(channel);
	channellist.insert(tolower(channel));
}

void IRCBot::OnChannelLeave(string channel)
{
	usermgr.SelfLeftChannel(channel);
	channellist.erase(tolower(channel));
}

void IRCBot::OnNickChange(string name)
{
	nick = name;
	lnick = tolower(nick);
}
