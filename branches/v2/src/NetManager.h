#ifndef _NET_MANAGER_H_
#define _NET_MANAGER_H_

#include <string>
#include <vector>
#include <queue>
#include <map>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/poll.h>

#include "EventHandler.h"
#include "Concurrency.h"

using namespace std;

class BotCore;
class NetInterface;

class NetManager
{
private:
	class NetBlob					// encapsulates various data about an interface
	{
	public:
		NetBlob();				// initializes some stuff

		// shared
		string netname;				// the interface's identifier (connectionindex[netname] will be the index of this NetBlob in connections)
		string outerindex;

		// owned by read thread
		NetInterface * iface;			// this connection
		char * readbuffer;			// a block of memory (1KB or so?) in which to store any fragment of data between reads
		int datasize;				// the size of any data in the read buffer (data will be located in beginning of buffer)
		int bufsize;				// the size of the buffer (might be constant for all buffers and thus unnecessary to store this)
		mtime_t ts;				// timestamp of fragmented message in readbuffer (if one exists)

		// owned by write thread
		// Timer writerestrictor;		// provides timing information for rate limiting // not implemented yet, start with no rate limiting
		Mutex queuelock;			// the write queue lock for this connection
		queue<string> write;			// write queue for this connection
	};

	class ReadThreadState				// various information pertaining to the process of reading from the network list
	{
	public:
		ReadThreadState(NetManager * parent);

		int servicepipe_in;			// service pipe endpoints
		int servicepipe_out;

		Mutex readmutex;			// always locked when read thread is running
		Condition servicelock;			// read thread will block on this condition when it pauses for administrivia, and unlock readmutex
		vector<pollfd> polllist;		// data required by poll syscall

		volatile bool running;				// running indicator
		pthread_t readthread;			// the read thread, responsible for all network input (communicates with the rest of the bot with events)
	};

	class WriteThreadState				// various information pertaining to the process of writing to the network list
	{
	public:
		WriteThreadState(NetManager * parent);

		queue<string> servicequeue;		// the service queue (similar purpose to service pipe)

		Mutex writemutex;			// the mutex to protect write operations
		Condition servicelock;			// the condition which halts the write thread when there is nothing to write
		Semaphore writespending;		// the number of pending writes, a managed atomic integer.  google semaphore
		Mutex squeuelock;			// the service queue lock

		volatile bool running;				// running indicator
		pthread_t writethread;			// the write thread, responsible for all network output (receives write requests as events)
	};

	ReadThreadState readstate;			// information and state specific to read thread
	WriteThreadState writestate;			// information and state specific to read thread

	BotCore& bot;					// the bot instance (doorway to much core functionality)

	vector<NetBlob *> connections;			// list of connections
	map<string, int> connectionindex;		// translator table from external to internal ids
	int nextavail;					// the next available id

	Mutex connectionlock;				// this mutex is locked whenever the event thread is accessing the connection table,
	Mutex servicemutex;				// this mutex is locked whenever something... its not used yet and i dont remember why i added it

	Condition writelock;				// blocks the write thread, signalling this condition indicates there is data to be written

	// property of read thread
	static void * DoReadService(void * object);	// static entry point for read thread
	void ReadService();				// object entry point for read thread
	void CompileReadPollList();			// initializes list of file descriptors (polllist) in ReadThreadState
	int WaitForRead();				// polls the list of descriptors, returns the index of the descriptor in polllist that has data
	void ReadIncomingData(int i);			// reads data from the interface specified by the netblob at index i and generates an event if a whole line is received
							// 0 indicates data in the service pipe, all others indicate data in a connection

	void WriteWaitingData();
	// property of write thread
	static void * DoWriteService(void * object);	// static entry point for write thread
	void WriteService();				// object entry point for write thread

	// property of event thread (other threads should not call)
	static void SendHandler(void * self, EventHandler::Event e);		// Handles incoming events containing instructions to send data to network
	static void ConnectionHandler(void * self, EventHandler::Event e);	// Handles incoming events signaling or instructing changes to network interfaces

	// unsorted
	void InitialAutoconnect();						// called upon receiving PROP_READY (indicating properties have been loaded), connects to all autoconnect networks
	void AddMessageToWriteQueue(string network, string line);		// Receives an event, and writes it to the write queue of some interface to be processed by write thread

	void PauseReadWrite();				// pause reader and writer threads
	void UnpauseReadWrite();			// don't forget to unpause them when you're done or everything ever will explode

public:
	NetManager(BotCore& b);				// constructor
	~NetManager();					// destructor

	void Start();					// starts component threads
	void Stop();					// stops component threads

	int GetConnectionIndex(string network);		// looks up connection interface index from external id
	string GetConnectionName(string network);

	void SubscribeToEvents();
};

#endif
