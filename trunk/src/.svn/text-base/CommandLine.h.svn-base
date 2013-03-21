#ifndef _COMMANDLINE_H_
#define _COMMANDLINE_H_

#include <set>
#include <string>

#include <stdlib.h>

using namespace std;

class BotInfo;

namespace cmdline
{

	void Terminate(int i);
	void ErrorAndTerminate(string message, int i);
	void DisplayHelpMessageAndQuit();

	void ThrottleSettings(BotInfo& b, string settings);

	void PutOption(BotInfo& info, string key, string value, int linenumber, set<string>& configfiles);
	void ReadConfigFile(BotInfo& info, string cfilename, set<string>& configfiles);
	void ReadCommandLineOptions(int argc, char * argv[], BotInfo& info);
}

#endif
