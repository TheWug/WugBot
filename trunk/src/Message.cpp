#include <string>
#include <vector>
#include <iostream>

#include "Message.h"
#include "misc.h"
#include "Hostname.h"

using namespace std;

Message::Message(string oldmessage)
{
	vector<string> data = splitcommands(oldmessage, " ");
	if (data.size() < 2)
	{
		Cancel();
		return;
	}
	type = tolower(data.at(0));
	channel = tolower(data.at(1));
	data = splitcommands(oldmessage, ":");
	message = oldmessage.substr(data.at(0).length() + 1, oldmessage.length() - data.at(0).length() - 1);
}

Message::Message(Hostname& target, string nmessage)
{
	type = "PRIVMSG";
	channel = target.GetNick();
	message = nmessage;
}

void Message::Cancel()
{
	type = "";
	channel = "";
	message = "";
}

string Message::GetType()
{
	return type;
}

string Message::GetChan()
{
	return channel;
}

string Message::GetContent()
{
	return message;
}

void Message::SetType(string newstr)
{
	type = newstr;
}

void Message::SetChan(string newstr)
{
	channel = newstr;
}

void Message::SetContent(string newstr)
{
	message = string(newstr.c_str());
}

string Message::GetMessageText()
{
	return type + " " + channel + " :" + message + "\r\n";
}

Message::operator string()
{
	return type + " " + channel + " :" + message + "\r\n";
}
