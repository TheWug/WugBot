#ifndef _CONCURRENCY_H_
#define _CONCURRENCY_H_

#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

class Mutex
{
private:
	pthread_mutex_t mutex;

public:
	Mutex();
	~Mutex();

	void Lock();
	void Unlock();

	pthread_mutex_t * Get();
};

class Semaphore
{
private:
	sem_t semaphore;

	Semaphore operator++ (int); // dummies: not defined.  don't try to use them, it doesn't make sense
	Semaphore operator-- (int); 

public:
	Semaphore();
	Semaphore(int initial);
	~Semaphore();

	Semaphore& operator++ (); // post operation
	Semaphore& operator-- (); // wait operation

	operator int(); // getvalue operation
};

class Condition
{
private:
	pthread_cond_t condition;

public:
	Condition();
	~Condition();

	void Wait(Mutex& mutex);
	void Signal();
	void Broadcast();
};

#endif
