#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <string.h>

#include "CommandLine.h"
#include "BotInfo.h"
#include "misc.h"

using namespace std;

namespace cmdline
{
	void ThrottleSettings(BotInfo& b, string settings)
	{
		vector<string> floodsettings = splitcommands(settings, ", ", true);
		int s = floodsettings.size();
		if (s > 0) b.SetFloodMinInterval(floodsettings.at(0));
		if (s > 1) b.SetFloodEventWeight(floodsettings.at(1));
		if (s > 2) b.SetFloodMaxWeight(floodsettings.at(2));
	}

	void Terminate(int i)
	{
		exit(i);
	}

	void ErrorAndTerminate(string message, int i)
	{
		cout << message << endl;
		Terminate(i);
	}

	void DisplayHelpMessageAndQuit()
	{
		cout << "Usage: IRCBot [--config-file <configfile>] [--connect-commands <connectcommandfile>] [-s <server:port>] [-n <nickname>] [-p <nickserv password>] [-a <nickserv account (if different than )>] [-r <quoted real name>] [-u <username>] [-o <owner>]" << endl;
		cout << "Command line arguments are not mandatory and will be processed in the order in which they are specified.  An option present twice will be updated by the first occurrence, which will then be overridden by the second." << endl;
		cout << "\tIf --config-file is not specified, its default is \"IRCBot.properties\"." << endl;
		cout << "\tIf --connect-commands is not specified, its default is \"IRCBot.cc\"." << endl;
		cout << "IRCBot.properties contains lines of the format \"keyname=key value etc etc // comment\"" << endl;
		cout << "IRCBot.properties can contain the following keys (others will be ignored):" << endl;
		cout << "nick user nspass realname server connectcommands owner" << endl;
		cout << "IRCBot.connect contains lines which are valid bot commands (with no command prefix)" << endl;
		cout << "\tExample: join #somechannel" << endl;
		Terminate(0);
	}

	void PutOption(BotInfo& info, string key, string value, int linenumber, set<string>& configfiles)
	{
		key = tolower(key);
		if (key == "config-file")
			ReadConfigFile(info, value, configfiles);
		else if (key == "connect-commands")
			info.SetConnectCommands(value);
		else if (key == "server")
			info.SetServer(value);
		else if (key == "nickname")
			info.SetNick(value);
		else if (key == "altnickname")
			info.SetAltNick(value);
		else if (key == "nspassword")
			info.SetPass(value);
		else if (key == "nsaccount")
			info.SetAccount(value);
		else if (key == "realname")
			info.SetRealname(value);
		else if (key == "username")
			info.SetUser(value);
		else if (key == "owner")
			info.SetOwner(value);
		else if (key == "prefix")
			info.SetCommandPrefix(value);
		else if (key == "throttle")
			ThrottleSettings(info, value);
		else if (key == "logchannel")
			info.SetLogChannel(value);
		else
			cout << "Error on line " << linenumber << ": invalid option specified" << endl;
		return;
	}

	void ReadConfigFile(BotInfo& info, string cfilename, set<string>& configfiles)
	{
		if (configfiles.find(cfilename) != configfiles.end())
		{
			cout << "Error: recursively specified config file (" << cfilename << ")" << endl;
			return;
		}
		ifstream cfile(cfilename.c_str(), ios::in);
		string line;
		string key, value;
		int linenum = 1;
		int index;
		if (cfile.is_open())
		{
			while (cfile.good())
			{
				getline(cfile, line);
				line = trim(line.substr(0, line.find("//")));
				if (line == "")
					continue;
				index = line.find('=');
				if (index == -1)
				{
					cout << "Error on line " << linenum << ": ignored key with no value" << endl;
					continue;
				}
				key = trim(line.substr(0, index));
				value = trim(line.substr(index + 1));
				PutOption(info, key, value, linenum, configfiles);
				++linenum;
			}
			cfile.close();
		}
		else
			cout << "Unable to read settings from file: \"" << cfilename << "\" (perhaps it does not exist?)" << endl;
		configfiles.erase(cfilename);
		return;
	}

	string GetArg(char * argv[], int limit, int argnum)
	{
		if (argnum >= limit)
			return "";
		return argv[argnum];
	}

	void ReadCommandLineOptions(int argc, char * argv[], BotInfo& info)
	{
		set<string> s;
		int i = 1;
		if (i % 2 == 0)
			ErrorAndTerminate("Improperly formatted command line arguments.", 2);
		while (i < argc)
		{
			string argi = tolower(GetArg(argv, argc, i));
			if ((argi == "-cf") || (argi == "--config-file"))
			{
				ReadConfigFile(info, GetArg(argv, argc, i + 1), s);
				i += 2;
			}
			else if ((argi == "-cc") || (argi == "--connect-commands"))
			{
				info.SetConnectCommands(GetArg(argv, argc, i + 1));
				i += 2;
			}
			else if ((argi == "-s") || (argi == "--server"))
			{
				info.SetServer(GetArg(argv, argc, i + 1));
				i += 2;
			}
			else if ((argi == "-n") || (argi == "--nickname"))
			{
				info.SetNick(GetArg(argv, argc, i + 1));
				i += 2;
			}
			else if ((argi == "-an") || (argi == "--altnickname"))
			{
				info.SetAltNick(GetArg(argv, argc, i + 1));
				i += 2;
			}
			else if ((argi == "-p") || (argi == "--nspassword"))
			{
				info.SetPass(GetArg(argv, argc, i + 1));
				i += 2;
			}
			else if ((argi == "-a") || (argi == "--account"))
			{
				info.SetAccount(GetArg(argv, argc, i + 1));
				i += 2;
			}
			else if ((argi == "-r") || (argi == "--realname"))
			{
				info.SetRealname(GetArg(argv, argc, i + 1));
				i += 2;
			}
			else if ((argi == "-u") || (argi == "--username"))
			{
				info.SetUser(GetArg(argv, argc, i + 1));
				i += 2;
			}
			else if ((argi == "-o") || (argi == "--owner"))
			{
				info.SetOwner(GetArg(argv, argc, i + 1));
				i += 2;
			}
			else if ((argi == "-cp") || (argi == "--commandprefix"))
			{
				info.SetCommandPrefix(GetArg(argv, argc, i + 1));
				i += 2;
			}
			else if ((argi == "-t") || (argi == "--throttle"))
			{
				ThrottleSettings(info, GetArg(argv, argc, i + 1));
				i += 2;
			}
			else if ((argi == "-l") || (argi == "--logchannel"))
			{
				info.SetLogChannel(GetArg(argv, argc, i + 1));
				i += 2;
			}
			else if ((argi == "-nd") || (argi == "--nodaemon"))
			{
				info.SetUseDaemon(false);
				++i;
			}
			else if ((argi == "-ro") || (argi == "--readonly"))
			{
				info.SetReadOnly("true");
				++i;
			}
		}
	}
};
