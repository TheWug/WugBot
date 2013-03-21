#include <sstream>
#include <fstream>

#include "WordFilter.h"
#include "IRCBot.h"
#include "misc.h"

WordFilterRule::WordFilterRule(WordFilter * f) : filter(f), refcount(0) {}

void WordFilterRule::Execute(Hostname& speaker, string& target, string& respondto)
{
	for (vector<string>::iterator c = toexecute.begin(); c != toexecute.end(); c++) // trollface
	{
		int i = 0, j = 0;
		string commandstring = *c;
		commandstring = stringreplace(
				stringreplace(
				stringreplace(
				stringreplace(
				stringreplace(
				stringreplace(	commandstring, "%%", "\t")
						, "%n", speaker.GetNick())
						, "%h", speaker.GetHost())
						, "%t", target)
						, "%r", respondto)
						, "\t", "%");
		filter->bot.PerformCommand(speaker, target, respondto, commandstring);
	}
}

void WordFilterRule::Add(string commandstring, int n)
{
	if (n < 0 || n >= toexecute.size())
		toexecute.push_back(commandstring);
	else
		toexecute.insert(toexecute.begin() + n, commandstring);
}

void WordFilterRule::Remove(int n)
{
	if (n >= 0 || n < toexecute.size())
		toexecute.erase(toexecute.begin() + n);
}

void WordFilterRule::Print(Hostname& speaker, Messenger respondto, bool override)
{
	if (toexecute.size() > 3 && !override)
	{
		respondto.Say("Too many lines! (" + tostr((int) toexecute.size()) + ") You may still use this command (via PM only) if you have privileged access.");
	}
	else
	{
		if (toexecute.size() > 3)
		{
			Messenger derp(filter->GetMessenger(speaker.GetNick()));
			for (vector<string>::iterator i = toexecute.begin(); i != toexecute.end(); ++i)
				derp.Say(*i);
		}
		else
		{
			for (vector<string>::iterator i = toexecute.begin(); i != toexecute.end(); ++i)
				respondto.Say(*i);
		}
	}
}

//const char WordFilter::crash[5] = {0xF3, 0xA0, 0x81, 0x9E, 0x0};

extern "C" module * Factory(IRCBot& bot)
{
	return new WordFilter(bot);
}

WordFilter::WordFilter(IRCBot& bot) : module(bot), rulelisthint(0) {}

void WordFilter::onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag)
{
	args = tolower(args);
	if ((command == "privmsg" || command == "notice") && target.length() > 0 && target.at(0) == '#' && allowedchans.find(tolower(target)) != allowedchans.end())
	{
		StringMatchTree<int>& chantree = settings[target];
		for (int i = 0; i < args.length(); ++i)
		{
			int& i1 = chantree.Match(args.substr(i));
			int& i2 = globalsettings.Match(args.substr(i));
			WordFilterRule& rule1 = (&i1 == NULL ? *((WordFilterRule *) NULL) : GetRule(i1));
			WordFilterRule& rule2 = (&i2 == NULL ? *((WordFilterRule *) NULL) : GetRule(i2));
			string r = respondto.GetTarget();
			if (&i1 != NULL && &rule1 == NULL)
				chantree.Clear(args.substr(i));
			else
				if (&rule1 != 0) { rule1.Execute(speaker, target, r); break; }

			if (&i2 != NULL && &rule2 == NULL)
				globalsettings.Clear(args.substr(i));
			else
				if (&rule2 != 0) { rule2.Execute(speaker, target, r); break; }
		}
	}
}

void WordFilter::onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto)
{
	string subject = target;
	bool validsubject = false, global = false;
	if (args.length() != 0 && args.at(0) == '#' || args.find("global") == 0)
	{
		istringstream s(args);
		s >> subject;
		args = trim(getline(s));
	}
	subject = tolower(subject);
	global = (subject == "global");
	validsubject = (subject.length() != 0 && subject.at(0) == '#') || global;
	if (controlstring == "help")
	{
		if (tolower(args) != "please")
			respondto.Say("The output of this command is very large.  use 'help please' to view.");
		else
			respondto.Say(	"WordFilter Module Commands (access with \"module wordfilter <command> [args]\")):\n"
				"  help\n"
				"    prints this monolithic block of text\n"
				"  allow [#channel]\n"
				"    enables use of wordfilter in the specified channel\n"
				"  deny [#channel]\n"
				"    disables use of wordfilter in the specified channel\n"
				"  newrule [#channel | global] <rule text>\n"
				"    creates a new rule and sets it as the active rule.\n"
				"    if location is not specified, it will default to the channel it is used in.\n"
				"    if used in pm, location must be specified.\n"
				"    if location is 'global', the rule will apply to all channels word filtering is allowed in.\n"
				"  deleterule [#channel | global] <rule text>\n"
				"    cancels the rule with the specified text from the specified location.\n"
				"    if the the removed rule is your active rule, your active rule is reset.\n"
				"  clonerule <new match string>\n"
				"    creates a reference to the active rule for the provided match string.\n"
				"    use this to create a single rule that applies to more than one filter or that exists in more than one channel.\n"
				"  select [#channel | global] <rule text>\n"
				"    selects the specified rule as the active rule, effectively opening it for editing.\n"
				"  add [index] <command>\n"
				"    adds the specified action to the active rule.\n"
				"    if index is specified, the action will be inserted into the list at the index-th position and all subsequent entries shifted down by one.\n"
				"    in 'command', the following substitutions are valid: %n: the speaker's nick, %h: the speaker's host, %t: the channel, %r: the response target, %%: a literal percent sign\n"
				"  remove [index]\n"
				"    removes the last action from the active rule.\n"
				"    if index is specified, the index-th action will be removed instead of the last, and all subsequent entries shifted up by one.\n"
				"  print\n"
				"    prints the action list for the active rule.\n"
				"  matchtest <match string>\n"
				"    prints a list of all rules that match the specified test string.  [NYI]\n");
	}
	if (controlstring == "allow")
	{
		if (!validsubject || global) { respondto.Say("You must specify a channel."); return; }
		allowedchans.insert(subject);
		respondto.Say("Allowing automatic filtering in " + subject);
	}
	else if (controlstring == "deny")
	{
		if (!validsubject || global) { respondto.Say("You must specify a channel."); return; }
		allowedchans.erase(subject);
		respondto.Say("Disabling automatic filtering in " + subject);
	}
	else if (controlstring == "newrule")
	{
		if (!validsubject) { respondto.Say("You must specify a channel."); return; }
		args = tolower(args);
		if (args == "") { respondto.Say("You cannot make a rule for the empty string."); return; }
		StringMatchTree<int>& tree = (subject == "global" ? globalsettings : settings[subject]);
		if (&(tree.Get(args)) != NULL) { respondto.Say("Rule already exists."); return; }
		int rule = GetNewRule();
		GetRule(rule).refcount++;
		tree.Set(args, rule);
		SetActiveRuleIndex(speaker.GetNickL(), rule);
		if (subject == "global") respondto.Say("Global rule created, active rule set.");
		else respondto.Say("Rule created for " + subject + ", active rule set.");
	}
	else if (controlstring == "deleterule")
	{
		bool wasselected = false;
		if (!validsubject) { respondto.Say("You must specify a channel."); return; }
		args = tolower(args);
		StringMatchTree<int>& tree = (subject == "global" ? globalsettings : settings[subject]);
		int& rule = tree.Get(args);
		if (&rule == NULL) { respondto.Say("Rule does not exist."); return; }
		
		int r = rule;
		tree.Clear(args);
		GetRule(r).refcount--;
		if (GetRule(r).refcount == 0)
		{
			respondto.Say("No remaining references, rule has been deleted.");
			DisposeRule(r);
		}
		else
			respondto.Say("Rule removed from " + subject + ".");
	}
	else if (controlstring == "select")
	{
		if (!validsubject) { respondto.Say("You must specify a channel."); return; }
		args = tolower(args);
		StringMatchTree<int>& tree = (subject == "global" ? globalsettings : settings[subject]);
		int& createdrule = tree.Get(args);
		if (&createdrule == NULL) { respondto.Say("Rule does not exist."); return; }
		SetActiveRuleIndex(speaker.GetNickL(), createdrule);
		respondto.Say("Selected rule.");
	}
	else if (controlstring == "add")
	{
		if (&GetActiveRule(speaker.GetNickL()) == NULL) { respondto.Say("You have no active rule."); return; }
		int i = -1;
		istringstream a(args);
		a >> i;
		a.clear();
		WordFilterRule& rule = GetActiveRule(speaker.GetNickL());
		string s = getline(a);
		rule.Add(s, i);
		respondto.Say("Added command to rule.");
	}
	else if (controlstring == "remove")
	{
		if (&GetActiveRule(speaker.GetNickL()) == NULL) { respondto.Say("You have no active rule."); return; }
		int i = -1;
		istringstream a(args);
		a >> i;
		WordFilterRule& rule = GetActiveRule(speaker.GetNickL());
		rule.Remove(i);
		respondto.Say("Removed command from rule.");
	}
	else if (controlstring == "print")
	{
		if (&GetActiveRule(speaker.GetNickL()) == NULL) { respondto.Say("You have no active rule."); return; }
		WordFilterRule& rule = GetActiveRule(speaker.GetNickL());
		rule.Print(speaker, respondto, true);
	}
	else if (controlstring == "clonerule")
	{
		if (!validsubject) { respondto.Say("You must specify a channel."); return; }
		if (args == "") { respondto.Say("You cannot make a rule for the empty string."); return; }
		int rule = GetActiveRuleIndex(speaker.GetNickL());
		if (rule == -1) { respondto.Say("You must have an active rule."); return; }
		args = tolower(args);
		StringMatchTree<int>& tree = (subject == "global" ? globalsettings : settings[subject]);
		if (&(tree.Get(args)) != NULL) { respondto.Say("Rule already exists."); return; }
		GetRule(rule).refcount++;
		tree.Set(args, rule);
		respondto.Say("Rule duplicated.");
	}
	else if (controlstring == "matchtest")
	{
		respondto.Say("Command not yet implemented, try again later :D");
	}
}

int WordFilter::GetNewRule()
{
	int i = rulelisthint;
	if (rulelisthint == rulelist.size())
	{
		rulelist.push_back(new WordFilterRule(this));
		AdvanceRuleListHint();
	}
	else
	{
		rulelist[rulelisthint] = new WordFilterRule(this);
		AdvanceRuleListHint();
	}
	return i;
}

void WordFilter::GetNewSpecificRule(int index)
{
	if (index <= rulelist.size())
		rulelist.resize(index + 1, NULL);
	if (rulelist[index] == NULL) rulelist[index] = new WordFilterRule(this);
	AdvanceRuleListHint();
}

void WordFilter::AdvanceRuleListHint()
{
	for (; rulelisthint < rulelist.size(); ++rulelisthint)
	{
		if (rulelist[rulelisthint] == NULL) break;
	}
}

void WordFilter::DisposeRule(int rule)
{
	for (map<string, int>::iterator i = activerules.begin(); i != activerules.end(); ++i)
		if (i->second == rule)
			i->second = -1;
	delete rulelist[rule];
	rulelist[rule] = NULL;
	if (rulelisthint > rule) rulelisthint = rule;
}

WordFilterRule& WordFilter::GetRule(int rule)
{
	if (rule >= rulelist.size())
		return *((WordFilterRule *)NULL);
	return *(rulelist[rule]);
}

WordFilterRule& WordFilter::GetActiveRule(string nick)
{
	map<string, int>::iterator i = activerules.find(nick);
	if (i == activerules.end())
		return *((WordFilterRule *)NULL);
	return GetRule(i->second);
}

void WordFilter::SetActiveRuleIndex(string nick, int rule)
{
	activerules[nick] = rule;
}

int WordFilter::GetActiveRuleIndex(string nick)
{
	map<string, int>::iterator i = activerules.find(nick);
	if (i == activerules.end())
		return -1;
	return i->second;
}

void WordFilter::LoadState()
{
	string channel;
	string line, token;
	int currentrule = -1;
	StringMatchTree<int> * tree = NULL;
	char buffer[4096]; buffer[0] = 0;

	ifstream pfile("modules/IRCBot.wordfilter");
	while (true)
	{
		pfile.getline(buffer, 4096);
		line = buffer;
		istringstream lstream(line);
		lstream >> token;

		if (!pfile) return;
		if (token == "[Rule]")
		{
			currentrule = -1;
			lstream >> currentrule;
			if (currentrule >= 0)
				GetNewSpecificRule(currentrule);
		}
		else if (token == "[Response]")
		{
			if (currentrule != -1)
				GetRule(currentrule).Add(getline(lstream), -1);
		}
		else if (token == "[Allowed]")
		{
			vector<string> chans = splitcommands(tolower(getline(lstream)), " ", false);
			for (int i = 0; i < chans.size(); ++i) allowedchans.insert(chans[i]);
		}
		else if (token == "[Global]")
		{
			tree = &globalsettings;
		}
		else if (token == "[Channel]")
		{
			tree = &(settings[tolower(getline(lstream))]);
		}
		else if (token == "[Match]")
		{
			int rule = -1;
			lstream >> rule;
			if (rule < 0 || tree == NULL) continue;
			tree->Set(getline(lstream), rule);
			GetRule(rule).refcount++;
		}
	}
}

void WordFilter::SaveState()
{
	ofstream pfile("modules/IRCBot.wordfilter");

	vector<int> adjustedvalues;
	int index = 0;
	for (vector<WordFilterRule *>::iterator i = rulelist.begin(); i != rulelist.end(); ++i)
	{
		if (*i == NULL)
		{
			adjustedvalues.push_back(-1);
			continue;
		}
		else
		{
			adjustedvalues.push_back(index);
			pfile << "[Rule] " << index << endl;
			for (vector<string>::iterator j = (*i)->toexecute.begin(); j != (*i)->toexecute.end(); ++j)
				pfile << "[Response] " << *j << endl;
			++index;
		}
	}

	pfile << "[Allowed]";
	for (set<string>::iterator i = allowedchans.begin(); i != allowedchans.end(); ++i) pfile << " " << *i;
	pfile << endl;
	
	pfile << "[Global]" << endl;
	for (StringMatchTree<int>::iterator i = globalsettings.begin(); i != globalsettings.end(); ++i)
	{
		pfile << "[Match] " << adjustedvalues[(*i).second] << " " << (*i).first << endl;
	}

	for (map<string, StringMatchTree<int> >::iterator i = settings.begin(); i != settings.end(); ++i)
	{
		pfile << "[Channel] " << i->first << endl;
		for (StringMatchTree<int>::iterator j = i->second.begin(); j != i->second.end(); ++j)
			pfile << "[Match] " << adjustedvalues[(*j).second] << " " << (*j).first << endl;
	}
	pfile.close();
}
