#include <unistd.h>
#include <cmath>
#include <iostream>

#include "Stat.h"
#include "LineDB.h"
#include "misc.h"

using namespace std;

bool Word::operator<(const Word& other) const
{
	if (weight < other.weight)
		return true;
	if (word < other.word)
		return true;
	return false;
}

const Word* Word::Find(set<Word>& s, string w)
{
	for (set<Word>::iterator i = s.begin(); i != s.end(); ++i)
	{
		if (i->word == w)
		{
			return &(*i);
		}
	}
	return 0;
}

Word::Word(string wo, float we)
{
	word = wo;
	weight = we;
}

Word::Word()
{
}

Word& Word::SetWord(string w)
{
	word = w;
	return *this;
}

Word& Word::SetWeight(float w)
{
	weight = w;
	return *this;
}

float WordNode::GetWeight(string testprev, string testnext)
{
	float value = .25;
	
	for (set<Word>::iterator p = previous.begin(); p != previous.end(); ++p)
	{
		if (p->word == testprev)
			value *= p->weight;
	}

	for (set<Word>::iterator n = next.begin(); n != next.end(); ++n)
	{
		if (n->word == testnext)
			value *= n->weight;
	}

	return value;
}

void WordNode::UpFrontWeight(string word)
{
	const Word * w = Word::Find(next, word);

	if (w == 0)
	{
		next.insert(Word(word, INITIALGOOD));
	}
	else if (w->weight + STEPVALUE < VMAX)
	{
		Word nw;
		nw.SetWeight(w->weight + STEPVALUE).SetWord(word);
		next.erase(*w);
		next.insert(nw);
	}
	if (next.size() > MAXSIZE)
	{
		const Word * lowest = 0;
		for (set<Word>::iterator i = next.begin(); i != next.end(); ++i)
		{
			if (lowest == 0 || lowest->weight < 1 || lowest->weight > i->weight)
				lowest = &(*i);
		}
		next.erase(*lowest);
	}
}

void WordNode::UpBackWeight(string word)
{
	const Word * w = Word::Find(previous, word);

	if (w == 0)
	{
		previous.insert(Word(word, INITIALGOOD));
	}
	else if (w->weight + STEPVALUE < VMAX)
	{
		Word nw;
		nw.SetWeight(w->weight + STEPVALUE).SetWord(word);
		previous.erase(*w);
		previous.insert(nw);
	}

	if (previous.size() > MAXSIZE)
	{
		const Word * lowest = 0;
		for (set<Word>::iterator i = previous.begin(); i != previous.end(); ++i)
		{
			if (lowest == 0 || lowest->weight < 1 || lowest->weight > i->weight)
				lowest = &(*i);
		}
		previous.erase(*lowest);
	}
}

void WordNode::DownFrontWeight(string word)
{
	const Word * w = Word::Find(next, word);

	if (w == 0)
		next.insert(Word(word, INITIALBAD));
	else if (w->weight - STEPVALUE > VMIN)
	{
		Word nw;
		nw.SetWeight(w->weight - STEPVALUE).SetWord(word);
		next.erase(*w);
		next.insert(nw);
	}

	if (next.size() > MAXSIZE)
	{
		const Word * lowest = 0;
		for (set<Word>::iterator i = next.begin(); i != next.end(); ++i)
		{
			if (lowest == 0 || lowest->weight < 1 || lowest->weight > i->weight)
				lowest = &(*i);
		}
		next.erase(*lowest);
	}
}

void WordNode::DownBackWeight(string word)
{
	const Word * w = Word::Find(previous, word);

	if (w == 0)
		previous.insert(Word(word, INITIALBAD));
	else if (w->weight - STEPVALUE > VMIN)
	{
		Word nw;
		nw.SetWeight(w->weight - STEPVALUE).SetWord(word);
		previous.erase(*w);
		previous.insert(nw);
	}

	if (previous.size() > MAXSIZE)
	{
		const Word * lowest = 0;
		for (set<Word>::iterator i = previous.begin(); i != previous.end(); ++i)
		{
			if (lowest == 0 || lowest->weight < 1 || lowest->weight > i->weight)
				lowest = &(*i);
		}
		previous.erase(*lowest);
	}
}

int WordNode::Entries()
{
	return previous.size() + next.size();
}

string WordNode::ToString(string word, WordNode node)
{
	const static string space = " ";
	string s;
	for (set<Word>::iterator i = node.previous.begin(); i != node.previous.end(); ++i)
	{
		s.append(i->word);
		s.append(space);
		s.append(tostr(i->weight));
		s.append(space);
	}
	s.append(';' + word + ';');
	for (set<Word>::iterator i = node.next.begin(); i != node.next.end(); ++i)
	{
		s.append(i->word);
		s.append(space);
		s.append(tostr(i->weight));
		s.append(space);
	}
	return s;
}

WordNode WordNode::FromString(string p, string n)
{
	WordNode w;
	istringstream fromprev(p);
	while (fromprev)
	{
		Word word;
		fromprev >> word.word;
		fromprev >> word.weight;
		if (word.word != "")
			w.previous.insert(word);
	}
	istringstream fromnext(n);
	while (fromnext)
	{
		Word word;
		fromnext >> word.word;
		fromnext >> word.weight;
		if (word.word != "")
			w.next.insert(word);
	}
	return w;
}

void FuzzyMatchDB::Add(string s)
{
	vector<string> words = splitcommands(s, " ", false);

	string next, prev;
	for (int i = 0; i < words.size(); ++i)
	{
		if (i == 0)
			prev = "&Begin";
		else
			prev = words[i - 1];
		if (i == words.size() - 1)
			next = "&End";
		else
			next = words[i + 1];
		database[words[i]].UpFrontWeight(next);
		database[words[i]].UpBackWeight(prev);
	}
}

void FuzzyMatchDB::Remove(string s)
{
	vector<string> words = splitcommands(s, " ", false);

	string next, prev;
	for (int i = 0; i < words.size(); ++i)
	{
		if (i == 0)
			prev = "&Begin";
		else
			prev = words[i - 1];
		if (i == words.size() - 1)
			next = "&End";
		else
			next = words[i + 1];
		database[words[i]].DownFrontWeight(next);
		database[words[i]].DownBackWeight(prev);
		if (database[words[i]].Entries() == 0)
			database.erase(words[i]);
	}
}

float FuzzyMatchDB::MatchWithString(string s)
{
	vector<string> words = splitcommands(s, " ", false);
	string next, prev;
	float f = 0;
	for (int i = 0; i < words.size(); ++i)
	{
		if (i == 0)
			prev = "&Begin";
		else
			prev = words[i - 1];
		if (i == words.size() - 1)
			next = "&End";
		else
			next = words[i + 1];
		map<string, WordNode>::iterator it = database.find(words[i]);
		if (it != database.end())
			f += it->second.GetWeight(prev, next);
	}
	return f / (pow(words.size(), .9) + 2.5);
}

int FuzzyMatchDB::Entries()
{
	return database.size();
}

extern "C" module * Factory(IRCBot& bot)
{
	return new Stat(bot);
}

Stat::Stat(IRCBot& b) : module(b)
{
}

Stat::~Stat()
{
}

void Stat::onMessage(string command, Hostname& speaker, string target, Messenger respondto, string args, bool ctcpflag)
{
	string s = speaker.GetNickL();
	if (command == "privmsg" || command == "notice")
	{
		if (target.length() > 0 && (target.at(0) == '+' || target.at(0) == '@'))
			target = target.substr(1);
		GetUserLineDB(s).AddLine(command, args);
		if (target.length() == 0 || target.at(0) != '#')
		{
			channeldatabases["@"].AddLine(command, args, speaker.GetNick());
		}
		else
		{
			channeldatabases[target].AddLine(command, args, speaker.GetNick());
		}
	}
	else if (command == "nick")
	{
		userdatabases[target] = userdatabases[s];
		userdatabases.erase(s);
		s = target;
		userdatabases[s].AddLine(command, s);
	}
	else if (command != "")
	{
		userdatabases[s].AddLine(command, args);
		channeldatabases[target].AddLine(command, args, speaker.GetNick());
	}
}

LineDB<8>& Stat::GetUserLineDB(string object)
{
	return userdatabases[object];
}

LineDB<25>& Stat::GetTargetLineDB(string object)
{
	return channeldatabases[object];
}

void Stat::onCommand(string controlstring, string args, Hostname& speaker, string target, Messenger respondto)
{
}

void Stat::LoadState()
{
}

void Stat::SaveState()
{
}
