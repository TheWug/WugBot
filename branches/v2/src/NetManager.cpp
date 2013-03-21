#include "NetManager.h"

#include <exception>
#include <sys/poll.h>
#include <errno.h>
#include <cstring>

#include "EventHandler.h"
#include "NetInterface.h"
#include "CommonEvents.h"
#include "StringUtil.h"
#include "BotCore.h"

NetManager::ReadThreadState::ReadThreadState(NetManager * parent)
{
	int fdesc[2];
	int e = pipe(fdesc);
	if (e == -1)
	{
		parent->bot.BotLog().GetLog(BotLogger::SYS).Put(CRITICAL, "NetManager::NetManager: Failed to open pipe! (errno: " + StringUtil::FromInt32(errno) + ")");
		servicepipe_in = -1;
		servicepipe_out = -1;
		return;
	}
	servicepipe_in = fdesc[0];
	servicepipe_out = fdesc[1];
	running = false;
	return;
}

NetManager::WriteThreadState::WriteThreadState(NetManager * parent)
{
	running = false;
	return;
}

NetManager::NetManager(BotCore& b) : bot(b), readstate(this), writestate(this)
{
	return;
}

NetManager::~NetManager()
{
	for (vector<NetBlob *>::iterator i = connections.begin(); i != connections.end(); ++i)
	{
		if ((*i) != NULL) delete (*i);
	}
	return;
}

void NetManager::Start()
{
	readstate.running = true;
	writestate.running = true;
	pthread_create(&readstate.readthread, NULL, DoReadService, this);
	pthread_create(&writestate.writethread, NULL, DoWriteService, this);
	return;
}

void NetManager::Stop()
{
	PauseReadWrite();
	readstate.running = false;
	writestate.running = false;
	UnpauseReadWrite();
	pthread_join(readstate.readthread, NULL);
	pthread_join(writestate.writethread, NULL);
	return;
}

void * NetManager::DoReadService(void * object)
{
	NetManager * self = ((NetManager *) object);
	self->readstate.readmutex.Lock(); // realistically placement of these here as opposed to in ReadService makes little difference but this way is more correct in general
	self->ReadService();
	self->readstate.readmutex.Unlock();
	return NULL;
}

void NetManager::ReadService()
{
	// intro section
	char dummy;

	// loop section
	while (true)
	{
		// block section
		CompileReadPollList(); // stores data in readstate.polllist
		int i = WaitForRead(); // reads data in readstate.polllist and returns the index of the first file descriptor it finds

		// service section
		if (i == 0) // if the service pipe has data
		{
			readstate.servicelock.Wait(readstate.readmutex); // wait for service to finish 
			read(readstate.servicepipe_in, &dummy, 1);
			continue;
		}
		else if (i < 0) continue;
		
		// general section
		ReadIncomingData(i - 1);
	}
	return;
}

void NetManager::ReadIncomingData(int index)
{
	NetBlob& blob = *connections[index];
	int readcount = blob.iface->Receive(blob.readbuffer + blob.datasize, blob.bufsize - blob.datasize - 1);
	if (readcount == -1) return;
	if (blob.datasize == 0) blob.ts = BotTime::GetCurrentTimeMillis();

	int totalsize = readcount + blob.datasize;
	blob.readbuffer[totalsize] = 0; // null terminate since it might not have been

	char * start = blob.readbuffer;
	char * end = strchr(blob.readbuffer, '\n');
	int consumed = 0;

	while (end != NULL)
	{
		consumed += end - start + 1;
		string line(start, end + 1);
		EventHandler::Event e;
		e.name = CommonEvents::NET_RECV;
		e.timestamp = blob.ts;
		e.data.push_back(blob.netname);
		e.data.push_back(line);
		bot.BotEvents().Raise(e);
		start = end + 1;
		end = strchr(blob.readbuffer, '\n');
		blob.ts = BotTime::GetCurrentTimeMillis();
	}

	if (consumed != 0)
	{
		totalsize -= consumed;
		memmove(blob.readbuffer, start, totalsize);
	}
	if (totalsize == blob.bufsize - 1)
	{
		bot.BotLog().GetLog(BotLogger::SYS).Put(ERROR, "NetManager::ReadIncomingData: read buffer overrun. THIS SHOULD NEVER EVER HAPPEN and probably means the read buffer is too small.");
		strcpy(blob.readbuffer, "GARBAGE :"); // causes data fragment to be ignored upon processing. 
		totalsize = 9;
	}

	blob.datasize = totalsize;
	return;
}

void NetManager::CompileReadPollList()
{
	if (readstate.polllist.size() < connections.size() + 1) readstate.polllist.resize(connections.size() + 1);
	readstate.polllist[0].fd = readstate.servicepipe_in;
	readstate.polllist[0].events = POLLIN;
	readstate.polllist[0].revents = 0;

	int openfds = 1;
	for (vector<NetBlob *>::iterator i = connections.begin(); i != connections.end(); ++i)
	{
		if ((*i)->iface->IsConnected())
		{
			(*i)->iface->Populate(readstate.polllist[openfds], POLLIN);
			++openfds;
		}
	}
	return;
}

int NetManager::WaitForRead()
{
	int i = poll(&readstate.polllist[0], readstate.polllist.size(), -1);
	if (i < 0) bot.BotLog().GetLog(BotLogger::SYS).Put(ERROR, "NetManager::WaitForRead: poll() failed.  Why did poll() fail? (errno: " + StringUtil::FromInt32(errno) + ")");

	else if (i > 0)
	{
		if (readstate.polllist[0].revents & POLLIN) return 0; // 0 is always service pipe
		for (int j = 0; j < readstate.polllist.size(); ++j)
		{
			if (readstate.polllist[j].revents & POLLIN) return j;
			if (readstate.polllist[j].revents & (POLLERR | POLLHUP | POLLNVAL)) connections[j - 1]->iface->Disconnect();
		}
	}
	return -1;
}

void * NetManager::DoWriteService(void * object)
{
	NetManager * self = ((NetManager *) object);
	self->writestate.writemutex.Lock();
	self->WriteService();
	self->writestate.writemutex.Unlock();
	return NULL;
}

void NetManager::WriteService()
{
	// intro section

	// loop section
	while(true)
	{
		// block section
		--writestate.writespending;

		// service section
		writestate.squeuelock.Lock();
		if (!writestate.servicequeue.empty())
		{
			writestate.servicequeue.pop();
			writestate.servicelock.Wait(writestate.writemutex);
			continue;
		}
		writestate.squeuelock.Unlock();

		// general section
		WriteWaitingData();
	}
	return;
}

void NetManager::WriteWaitingData() // CHANGEME - write only one data at a time
{
	for (int i = 0; i < connections.size(); ++i)
	{
		NetBlob& blob = *connections[i];
		blob.queuelock.Lock();
		while (blob.write.size() != 0)
		{
			blob.iface->Send(blob.write.front().c_str(), blob.write.front().length());
		}
		blob.queuelock.Unlock();
	}
	return;
}

void NetManager::SendHandler(void * s, EventHandler::Event e)
{
	NetManager * self = (NetManager *) s;
	if (!StringUtil::CaseInsEquals(e.name, CommonEvents::NET_SEND))
	{
		self->bot.BotLog().GetLog(BotLogger::DBG).Put(ERROR, "NetManager::SendHandler: Got an event that was not a data send! (ignored it)");
		return;
	}

	try
	{
		self->AddMessageToWriteQueue(e.data[0], e.data[1]);
	}
	catch (exception& ex)
	{
		self->bot.BotLog().GetLog(BotLogger::DBG).Put(ERROR, "NetManager::SendHandler: Error occurred processing output data.");
		self->bot.BotLog().GetLog(BotLogger::DBG).Put(ERROR, "NetManager::SendHandler: Exception:" + string(ex.what()));
	}
	return;
}

void NetManager::AddMessageToWriteQueue(string network, string line)
{
	int i = GetConnectionIndex(network);
	if (i == -1)
	{
		bot.BotLog().GetLog(BotLogger::SYS).Put(ERROR, "NetManager::AddMessageToWriteQueue: tried to queue data to nonexistent network \"" + network + "\" (message dropped)");
		return;
	}

	NetBlob& blob = *connections[i];
	blob.queuelock.Lock(); // lock individual write queue

	blob.write.push(line);

	blob.queuelock.Unlock(); // unlock individual write queue

	++writestate.writespending;
	return;
}

string NetManager::CreateIRCLine(vector<string>& components)
{
	if (components.size() < 2) return "";
	else if (components.size() == 2) return (components[1] + "\r\n");
	else
	{
		string line;

		for (int i = 1; i < components.size() - 1; ++i)
		{
			line += components[i];
			line += " ";
		}

		line += ":";
		line += components[components.size() - 1];
		line += "\r\n";
		return line;
	}
	bot.BotLog().GetLog(BotLogger::DBG).Put(CRITICAL, "NetManager::SendHandler: what the fucking fuck.");
	return "";
}

int NetManager::GetConnectionIndex(string network)
{
	map<string, int>::iterator i = connectionindex.find(network);
	if (i == connectionindex.end()) return -1;
	return i->second;
}

void NetManager::PauseReadWrite()
{
	write(readstate.servicepipe_out, "\0", 1); 		// this will wake up the read loop
	readstate.readmutex.Lock(); 	// read loop will pause itself if the service pipe has data
	// read thread is now paused

	writestate.squeuelock.Lock(); 			// lock service queue
	writestate.servicequeue.push(".");          	// add to service queue
	writestate.squeuelock.Unlock(); 		// unlock service queue
	++writestate.writespending;	 		// increment writes pending counter
	writestate.writemutex.Lock(); 			// write loop will pause itself if the service queue has an entry
	// write thread is now paused
	return;
}

void NetManager::UnpauseReadWrite()
{
	writestate.servicelock.Signal();	// notify the signal the write thread is waiting on
	writestate.writemutex.Unlock();		// unlock the write mutex
	readstate.servicelock.Signal(); 	// notify the signal the read thread is waiting on
	readstate.readmutex.Unlock();	  	// unlock the read mutex
	return;
}

void NetManager::ConnectionHandler(void * s, EventHandler::Event e)
{
	NetManager * self = (NetManager *) s;
	self->PauseReadWrite();

	// do work
	// what events should we be handling here
	// force send message now
	// add network to list
	// remove network from list
	// connect network
	// disconnect network
	// reconnect network
	// list consolidate

	self->UnpauseReadWrite();
	return;
}
