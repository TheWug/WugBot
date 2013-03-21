#include <string>

#include "Hostname.h"
#include "misc.h"

using namespace std;

const string Hostname::unauthenticated = "*";
const string Hostname::undetermined = "";

Hostname::Hostname()
{
	Cancel();
}

Hostname::Hostname(string n, string i, string h, string u)
{
	nick = n;
	user = i;
	host = h;
	account = u;
	lnick = tolower(nick);
	lhost = tolower(host);
	laccount = tolower(account);
	luser = tolower(user);
}

Hostname::Hostname(string name)
{
	if (name.length() == 0 || name == "*")
	{
		Cancel();
		return;
	}
	if (name.at(0) == '#')
	{
		host = name;
		lhost = name;
		nick = name;
		lnick = tolower(name);
		account = "";
		laccount = "";
		user = name;
		luser = name;
		return;
	}
	vector<string> step1 = splitcommands(name, "@");
	if (step1.size() != 2)
	{
		host = name;
		lhost = tolower(host);
		nick = name;
		lnick = tolower(name);
		user = name;
		luser = name;
		account = "";
		laccount = "";
		return;
	}
	account = "";
	laccount = "";
	host = step1.at(1);
	lhost = tolower(host);
	vector<string> step2 = splitcommands(step1.at(0), "!");
	if (step2.size() != 2)
	{
		Cancel();
		return;
	}
	nick = step2.at(0);
	lnick = tolower(nick);
	user = step2.at(1);
	luser = tolower(user);
}

void Hostname::Cancel()
{
	host = "";
	lhost = "";
	nick = "";
	lnick = "";
	account = "";
	laccount = "";
	user = "";
	luser = "";
}

string Hostname::GetFullHostname()
{
	return nick + "!" + user + "@" + host;
}

string Hostname::GetNick()
{
	return nick;
}

string Hostname::GetNickL()
{
	return lnick;
}

string Hostname::GetUser()
{
	return user;
}

string Hostname::GetUserL()
{
	return luser;
}

string Hostname::GetHost()
{
	return host;
}

string Hostname::GetHostL()
{
	return lhost;
}

string Hostname::GetAccount()
{
	return account;
}

string Hostname::GetAccountL()
{
	return laccount;
}

void Hostname::SetNick(string newstr)
{
	nick = newstr;
	lnick = tolower(nick);
}

void Hostname::SetUser(string newstr)
{
	user = newstr;
	luser = tolower(user);
}

void Hostname::SetHost(string newstr)
{
	host = newstr;
	lhost = tolower(host);
}

void Hostname::SetAccount(string newstr)
{
	if (newstr == "0" || newstr == "")
		newstr = "*";
	account = newstr;
	laccount = tolower(account);
}
