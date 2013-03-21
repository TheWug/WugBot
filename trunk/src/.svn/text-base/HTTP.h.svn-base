#ifndef _HTTPREQUEST_H_
#define _HTTPREQUEST_H_

#include <map>
#include <string>

using namespace std;

class HTTPRequest
{
	public:
	string host;
	string server;
	string page;
	map<string, string> get;

	HTTPRequest(string url);
	HTTPRequest();

	int Do();
	bool SetTarget(string url);

	string GetHeader() const;
	string GetResponse() const;
	int GetCode() const;

	private:
	int code;
	string response;
	string header;
};

#endif
