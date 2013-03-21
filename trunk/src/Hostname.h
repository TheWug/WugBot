#ifndef _HOSTNAME_H_
#define _HOSTNAME_H_

#include <string>

using namespace std;

class Hostname
{
	private:
	string nick;
	string lnick;

	string user;
	string luser;

	string account;
	string laccount;

	string host;
	string lhost;

	public:
	Hostname(string nick, string ident, string host, string user);
	Hostname(string name);
	Hostname();

	string GetNick();
	string GetNickL();
	string GetUser();
	string GetUserL();
	string GetAccount();
	string GetAccountL();
	string GetHost();
	string GetHostL();

	string GetFullHostname();

	void SetNick(string newstr);
	void SetUser(string newstr);
	void SetAccount(string newstr);
	void SetHost(string newstr);

	void Cancel();

	static const string unauthenticated;
	static const string undetermined;
};

#endif
