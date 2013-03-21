#include "Concurrency.h"

#include <errno.h>

// no error handling here at all because some people like to watch the world burn

Mutex::Mutex()
{
	pthread_mutex_init(&mutex, NULL);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&mutex);
}

void Mutex::Lock()
{
	int i = pthread_mutex_lock(&mutex);
	if (i != 0) throw i;
}

void Mutex::Unlock()
{
	int i = pthread_mutex_unlock(&mutex);
	if (i != 0) throw i;
}

pthread_mutex_t * Mutex::Get()
{
	return &mutex;
}

Semaphore::Semaphore()
{
	sem_init(&semaphore, 0, 1);
}

Semaphore::Semaphore(int initial)
{
	sem_init(&semaphore, 0, initial);
}

Semaphore::~Semaphore()
{
	sem_destroy(&semaphore);
}

Semaphore& Semaphore::operator++ ()
{
	int i, j;
	i = errno;
	j = sem_post(&semaphore);
	if (j != 0)
	{
		j = errno;
		errno = i;
		throw j;
	}
	errno = i;
	return (*this);
}

Semaphore& Semaphore::operator-- ()
{
	int i, j;
	i = errno;
	j = sem_wait(&semaphore);
	if (j != 0)
	{
		j = errno;
		errno = i;
		throw j;
	}
	errno = i;
	return (*this);
}

Semaphore::operator int()
{
	int i, j, k = -1;
	i = errno;
	j = sem_getvalue(&semaphore, &k);
	if (j != 0)
	{
		j = errno;
		errno = i;
		throw j;
	}
	errno = i;
	return k;
}

Condition::Condition()
{
	pthread_cond_init(&condition, NULL);
}

Condition::~Condition()
{
	pthread_cond_destroy(&condition);
}

void Condition::Wait(Mutex& mutex)
{
	int i = pthread_cond_wait(&condition, mutex.Get());
	if (i != 0) throw i;
}

void Condition::Signal()
{
	int i = pthread_cond_signal(&condition);
	if (i != 0) throw i;
}

void Condition::Broadcast()
{
	int i = pthread_cond_broadcast(&condition);
	if (i != 0) throw i;
}

