#ifndef _MEMOSENDER_H_
#define _MEMOSENDER_H_

#include <vector>
#include <string>
#include <map>
#include <set>

#include "dcmd.h"

using namespace std;

class Memo
{
	public:
	Memo(string b, string s, string t, int i);
	string GetMessage();

	string body;
	int id;
	string sender;
	string target;
};

class MemoSender : public dcmd
{
	public:
	MemoSender(IRCBot& bot);

	bool Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag);
	
	bool AddMemo(string user, string target, string memo);
	string ViewSent(string user, string target);
	bool DeleteSent(string user, int memoid);
	void DeleteTo(string user, string target);
	void Block(string user);
	void Unblock(string user);
	bool CanSend(string user);

	static string GetSenderString(Hostname sender);
	static string GetTargetString(string target);

	private:
	map<string, map<string, vector<Memo> > > memolist;
	set<string> blocklist;
	map<string, int> memoids;
};

#endif
