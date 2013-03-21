#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <string>

#include "Hostname.h"

using namespace std;

class Message
{
	private:
	string type;
	string channel;
	string message;
	string special;

	public:
	Message(string full);
	Message(Hostname& target, string message);
	Message() {}

	string GetType();
	string GetChan();
	string GetContent();

	void SetType(string newstr);
	void SetChan(string newstr);
	void SetContent(string newstr);

	string GetMessageText();

	void Cancel();

	operator string();
};

#endif
