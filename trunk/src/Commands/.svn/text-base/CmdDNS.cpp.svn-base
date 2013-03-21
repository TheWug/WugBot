#include <sys/socket.h>

#include "misc.h"
#include "IRCBot.h"
#include "CmdDNS.h"

CmdDNS::CmdDNS()
{
	defaultuserlevel = 0;
}

void CmdDNS::ProcessCommand(IRCBot& bot, string command, Hostname speaker, string target, string respondto, string args)
{
	hostent *hostentry = gethostbyname(args.c_str());
	if (hostentry == NULL)
		bot.Say(respondto, "Host " + args + " does not exist.");
	else
		bot.Say(respondto, GetIP(ntohl(((in_addr *) hostentry->h_addr)->s_addr)));
}

string CmdDNS::HelpMsg(string command)
{
	return "Usage: " + command + " <URL> -- performs a DNS query on the provided url and returns its IP address.  An error message is shown if the address cannot be resolved.";
}

vector<string> CmdDNS::CommandStrings()
{
	vector<string> types;
	types.push_back("dns");
	types.push_back("resolve");
	return types;
}
