#ifndef _WORDFILTER_H_
#define _WORDFILTER_H_

#include <map>
#include <set>
#include <vector>
#include <string>

#include "module.h"
#include "StringMatchTree.h"

using namespace std;

class WordFilter;

class WordFilterRule
{
	public:
	WordFilterRule(WordFilter * f);

	void Execute(Hostname& speaker, string& target, string& respondto);

	void Add(string commandstring, int n);
	void Remove(int n);
	void Print(Hostname& speaker, Messenger respondto, bool override);

	vector<string> toexecute;
	WordFilter * filter;

	int refcount;
};

extern "C" module * Factory(IRCBot& bot);

class WordFilter : public module
{
	public:

	WordFilter(IRCBot& bot);

	virtual void onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag);
	virtual void onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto);

	virtual void LoadState();
	virtual void SaveState();

	map<string, StringMatchTree<int> > settings;
	StringMatchTree<int> globalsettings;

	vector<WordFilterRule *> rulelist;
	int rulelisthint;

	map<string, int> activerules;

	set<string> allowedchans;

	int GetNewRule();
	void GetNewSpecificRule(int index);
	void DisposeRule(int rule);
	WordFilterRule& GetRule(int rule);
	WordFilterRule& GetActiveRule(string nick);
	int GetActiveRuleIndex(string nick);
	void SetActiveRuleIndex(string nick, int rule);
	void AdvanceRuleListHint();
};

#endif
