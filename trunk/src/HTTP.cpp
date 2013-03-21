#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fcntl.h>
#include <errno.h>
#include <sys/poll.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <pthread.h>

#include "IRCBotNetIface.h"
#include "HTTP.h"
#include "misc.h"

using namespace std;

HTTPRequest::HTTPRequest(string url)
{
	SetTarget(url);
	code = -1;
}
HTTPRequest::HTTPRequest()
{
	code = -1;
}

bool HTTPRequest::SetTarget(string url)
{
	int temp;
	int protocolmarker = url.find("://");
//	cout << url << endl;
//	cout << protocolmarker << endl;
	if (protocolmarker != string::npos)
	{
		protocolmarker += 3;
		url = url.substr(protocolmarker, string::npos);
	}
//	cout << url << endl;

	int hostmarker = url.find("/");
//	cout << "/";
	if (hostmarker == string::npos)
	{
//		cout << "?";
		temp = url.find("?");
		if (temp < hostmarker) hostmarker = temp;
	}
//	cout << " " << hostmarker << endl;

	if (hostmarker == string::npos)
		hostmarker = url.length();
	host = url.substr(0, hostmarker);
	server = host;
//	cout << server << endl;

	url = url.substr(hostmarker, url.length() - hostmarker);
//	cout << url << endl;

	int pathmarker = url.find("?");
//	cout << pathmarker << endl;
	if (pathmarker == string::npos)
	{
		page = "/";
		url = "";
//		cout << page << endl;
//		cout << url << endl;
	}
	else
	{
		page = url.substr(0, pathmarker);
		url = url.substr(pathmarker + 1, url.length() - pathmarker - 1);
//		cout << page << endl;
//		cout << url << endl;
	}
	vector<string> GETvalues = splitcommands(url, "&", false);
	for (int i = 0; i < GETvalues.size(); ++i)
	{
//		cout << GETvalues[i] << endl;
		vector<string> tokens = splitcommands(GETvalues[i], "=", false);

//		cout << tokens.size() << endl;
		if (tokens.size() > 1)
		{
//			cout << tokens[0] << ":" << tokens[1] << endl;
			get[tokens[0]] = tokens[1];
		}
		else if (tokens.size() > 0)
		{
//			cout << tokens[0] << endl;
			get[tokens[0]] = "";
		}
	}
	return true;
}

int HTTPRequest::Do()
{
	string buffer;
	string request = "";
	string document = trim(page);
	int c = 0;
	for (map<string, string>::iterator i = get.begin(); i != get.end(); ++i)
	{
		if (c == 0)
			document += "?";
		else
			document += "&";
		document += i->first;
		if (i->second != "")
		{
			document += "=";
			document += i->second;
		}
		++c;
	}

	request += "GET " + document + " HTTP/1.1";
	request += "\r\n";
	request += "Host: " + trim(host);
	request += "\r\n";
	request += "Connection: close";
	request += "\r\n";
	request += "User-Agent: WugBot";
	request += "\r\n";
	request += "\r\n";

	hostent *hostentry;
	sockaddr_in addr;
	int socketstate;

	hostentry = gethostbyname(server.c_str());
	socketstate = socket(AF_INET, SOCK_STREAM, 0);

	memset(&addr, 0, sizeof(sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	if (hostentry == 0)
	{
		close(socketstate);
		return -5;
	}
	addr.sin_addr.s_addr = ((in_addr *) hostentry->h_addr)->s_addr;
	if (addr.sin_addr.s_addr == 0)
	{
		close(socketstate);
		return -5;
	}

	int fstate = fcntl(socketstate, F_GETFL, NULL);
	if (fstate < 0)
	{
		close(socketstate);
		return -1;
	}

	if (fcntl(socketstate, F_SETFL, fstate | O_NONBLOCK) < 0)
	{
		close(socketstate);
		return -1;
	}
	
	if (!(connect(socketstate, (sockaddr *) &addr, sizeof(addr)) < 0 && errno == EINPROGRESS))
	{
		close(socketstate);
		return -3;
	}

	if (fcntl(socketstate, F_SETFL, fstate) < 0)
	{
		close(socketstate);
		return -1;
	}

	int value = IRCBotNetIface::WaitForSocket(socketstate, POLLOUT, 2000);
	if (value <= 0)
	{
		close(socketstate);
		return -2;
	}

	send(socketstate, request.c_str(), request.length(), 0);
	int received = 0;
	int size = 512;
	vector<char> b;
	b.resize(size);

	value = IRCBotNetIface::WaitForSocket(socketstate, POLLIN, 2000);
	if (value <= 0)
	{
		close(socketstate);
		return -2;
	}

	while (true)
	{
		int g = recv(socketstate, &(b[received]), size - received, 0);
		if (g > 0)
			received = received + g;
		if (received == size)
		{
			size *= 2;
			b.resize(size);
		}
		if (g == -1 || g == 0)
			break;
	}

	b[received] = 0;
	buffer = &(b[0]);
	close(socketstate);
	int seperation = buffer.find("\r\n\r\n");
	header = buffer.substr(0, seperation);
	if (seperation != string::npos && seperation != buffer.length() - 4)
		response = buffer.substr(seperation + 4);

	istringstream headerstream(header);
	string dummy;
	headerstream >> dummy;
	headerstream >> code;
	return code;
}

string HTTPRequest::GetResponse() const
{
	return response;
}

string HTTPRequest::GetHeader() const
{
	return header;
}

int HTTPRequest::GetCode() const
{
	return code;
}
