#include <iostream>
#include <fstream>
#include <set>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#include "IRCBot.h"
#include "BotInfo.h"
#include "IRC.h"
#include "Cmd.h"
#include "misc.h"
#include "CommandLine.h"

using namespace std;
using namespace cmdline;

IRCBot * bot = 0;
struct sigaction * SIGINThandler = 0;
struct sigaction * SIGSEGVhandler = 0;

void HandleCtrlC(int i)
{
	if (bot != 0)
	{
		bot->network.Send(IRC::Quit("Received Control-C from console"));
		bot->FinishConnect();
	}
	bot->~IRCBot();
	exit(1);
}

void HandleSegFault(int i)
{
	signal(SIGSEGV, SIG_DFL);
	if (bot != 0)
	{
		bot->network.Send(IRC::Quit("SEGMENTATION FAULT >:O"));
	}
	exit(1);
}

void ConfigureSignalHandlers()
{
	if (SIGINThandler == 0)
		SIGINThandler = new struct sigaction;
	if (SIGSEGVhandler == 0)
		SIGSEGVhandler = new struct sigaction;

	SIGINThandler->sa_handler = HandleCtrlC;
	sigemptyset(&(SIGINThandler->sa_mask));
	SIGINThandler->sa_flags = 0;

	sigaction(SIGINT, SIGINThandler, 0);
	sigaction(SIGTERM, SIGINThandler, 0);

	SIGSEGVhandler->sa_handler = HandleSegFault;
	sigemptyset(&(SIGSEGVhandler->sa_mask));
	SIGSEGVhandler->sa_flags = 0;

	sigaction(SIGSEGV, SIGSEGVhandler, 0);
}

int DaemonRise()
{
	cout << "Forking into the background..." << endl;
	pid_t p, s;
	p = fork();

	if (p < 0)
		return p;
	else if (p != 0)
		ErrorAndTerminate("", 0);

	umask(000);

	s = setsid();

	if (s < 0)
		return s;

	int output = open("./IRCBot.log", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	int input = open("/dev/null", O_RDWR);

	if (output < 0)
		return output;
	if (input < 0)
		return input;

	if (dup2(input, STDIN_FILENO) < 0)
		return -1;

	if (dup2(output, STDOUT_FILENO) < 0)
		return -1;

	if (dup2(output, STDERR_FILENO) < 0)
		return -1;

	return 0;
}

main(int argc, char * argv[])
{
	cout << "stuff" << endl;
	ConfigureSignalHandlers();

	cout << "stuff" << endl;

	BotInfo b;
	set<string> s;
	ReadConfigFile(b, "IRCBot.properties", s);
	ReadCommandLineOptions(argc, argv, b);

	cout << "stuff" << endl;

	if (!b.HasEnoughInfo())
	{
		ErrorAndTerminate("The bot's nickname and a server to connect to are manditory and must be specified.", 3);
	}

	cout << "stuff" << endl;

	if (b.GetUseDaemon())
		if (DaemonRise() < 0)
			ErrorAndTerminate("Unable to fork into background as daemon process.", 4);

	IRCBot WugBot(b);
	bot = &WugBot;

	cout << "Bot has been created." << endl;

	WugBot.InitialConnect(b.GetServer(), b.GetPort());
	WugBot.ProcessLoop();
	cout << "Connection has finished, terminating..." << endl;
	WugBot.FinishConnect();

	bot = 0;
	delete SIGINThandler;
	delete SIGSEGVhandler;

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	return 0; 
}
