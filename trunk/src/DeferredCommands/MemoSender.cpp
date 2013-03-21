#include <sstream>
#include <iomanip>

#include "MemoSender.h"
#include "IRCBot.h"
#include "misc.h"

Memo::Memo(string b, string s, string t, int i)
{
	body = b;
	sender = s;
	target = t;
	id = i;
}

string Memo::GetMessage()
{
	return "<" + sender.substr(2) + "> " + body;
}

MemoSender::MemoSender(IRCBot& bot) : dcmd(Hostname(""), "", bot)
{
}

bool MemoSender::Call(string command, Hostname speaker, string target, string respondto, string message, bool ctcpflag)
{
	if (command != "privmsg" && command != "notice")
		return false;
	map<string, map<string, vector<Memo> > >::iterator end = memolist.end();
	map<string, vector<Memo> >::iterator memostosend;
	string memostring = "";
	int nummemos = 0;
	for (map<string, map<string, vector<Memo> > >::iterator i = memolist.begin(); i != end; ++i)
	{
		memostosend = ((*i).second).find(GetSenderString(speaker));
		if (memostosend == ((*i).second).end())
			continue;
		vector<Memo>& temp = memostosend->second;
		for (int size = 0; size < temp.size(); ++size)
		{
			++nummemos;
			memostring += temp[size].GetMessage() + "\n";
		}
		((*i).second).erase(memostosend);
		if ((*i).second.size() == 0)
			memolist.erase(i);
	}
	if (memostring == "")
		return false;
	bot.Say(speaker.GetNick(), "You have " + tostr(nummemos) + " new memos.\n" + memostring);
	return false;
}

bool MemoSender::AddMemo(string user, string target, string memo)
{
	if (!CanSend(target))
		return false;
	int i = ++memoids[user];
	(memolist[user])[target].push_back(Memo(memo, user, target, i));
	return true;
}

string MemoSender::ViewSent(string user, string target)
{
	string s;
	for (map<string, vector<Memo> >::iterator i = memolist[user].begin(); i != memolist[user].end(); ++i)
	{
		if (target == "" || target == i->first)
		{
			for (vector<Memo>::iterator j = (i->second).begin(); j != (i->second).end(); ++j)
			{
				ostringstream ss;
				if (j->target.find("N:") == 0)
					ss << "To: " << setw(16) << left << j->target.substr(2)<< "[" << setw(4) << left << j->id << "] " << j->body << "\n";
				else
					ss << "To: [A] " << setw(12) << left << j->target.substr(2)<< "[" << setw(4) << left << j->id << "] " << j->body << "\n";
				s += ss.str();
			}
		}
	}
	return s;
}

bool MemoSender::DeleteSent(string user, int memoid)
{
	for (map<string, vector<Memo> >::iterator i = memolist[user].begin(); i != memolist[user].end(); ++i)
	{
		for (vector<Memo>::iterator j = (i->second).begin(); j != (i->second).end(); ++j)
		{
			if (j->id == memoid)
			{
				i->second.erase(j);
				if (i->second.size() == 0)
				{
					memolist[user].erase(i->first);
					if (memolist[user].size() == 0)
					{
						memolist.erase(user);
						memoids.erase(user);
					}
				}
				return true;
			}
		}
	}
	return false;
}

void MemoSender::DeleteTo(string user, string target)
{
	memolist[user].erase(target);
	if (memolist[user].size() == 0)
	{
		memolist.erase(user);
		memoids.erase(user);
	}
}

void MemoSender::Block(string user)
{
	blocklist.insert(user);
}

void MemoSender::Unblock(string user)
{
	blocklist.erase(user);
}

bool MemoSender::CanSend(string user)
{
	return blocklist.find(user) == blocklist.end();
}

string MemoSender::GetSenderString(Hostname sender)
{
	if (sender.GetAccountL() == Hostname::unauthenticated || sender.GetAccountL() == Hostname::undetermined)
	{
		//cout << "N:" + sender.GetNickL() << endl;
		return "N:" + sender.GetNickL();
	}
	//cout << "A: '" + sender.GetAccountL() << "'" << endl;
	return "A:" + sender.GetAccountL();
}

string MemoSender::GetTargetString(string target)
{
	if (target.length() == 0)
	{
		//cout << "nil" << endl;
		return "";
	}
	if (target[0] == '~')
	{
		//cout << "A:" + tolower(target.substr(1)) << endl;
		return "A:" + tolower(target.substr(1));
	}
	//cout << "N:" + tolower(target) << endl;
	return "N:" + tolower(target);
}
