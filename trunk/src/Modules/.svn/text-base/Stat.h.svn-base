#ifndef _STAT_H_
#define _STAT_H_

#include <string>
#include <map>
#include <set>
#include <list>

#include "module.h"
#include "LineDBClass.h"
#include "LineDB.h"

class Word
{
	public:
	Word(string wo, float we);
	Word();

	Word& SetWord(string w);
	Word& SetWeight(float w);

	bool operator<(const Word& other) const;

	static const Word* Find(set<Word>& s, string w);

	string word;
	float weight;
};

class WordNode
{
	set<Word> previous;
	set<Word> next;

	public:
	WordNode() {}

	float GetWeight(string testfront, string testback);
	static string ToString(string word, WordNode node);
	static WordNode FromString(string p, string n);

	void UpFrontWeight(string newword);
	void UpBackWeight(string newword);

	void DownFrontWeight(string newword);
	void DownBackWeight(string newword);

	int Entries();

	static const int MAXSIZE = 6;
	static const float INITIALGOOD = 1.25, INITIALBAD = 0.75, STEPVALUE = .25, VMAX = 3, VMIN = -1;
};

class FuzzyMatchDB
{
	public:
	map<string, WordNode> database;

	float MatchWithString(string s);

	void Add(string s);
	void Remove(string s);

	int Entries();
};

extern "C" module * Factory(IRCBot& bot);

class Stat : public module
{
	public:
	Stat(IRCBot& b);
	~Stat();

	map<string, LineDB<8> > userdatabases;
	map<string, LineDB<25> > channeldatabases;

	LineDB<8>& GetUserLineDB(string object);
	LineDB<25>& GetTargetLineDB(string object);

	void LoadState();
	void SaveState();

	void onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag);
	void onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto);
};

#endif
