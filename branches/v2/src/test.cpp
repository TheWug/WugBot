#include "Concurrency.h"
#include <unistd.h>

#include <iostream>
#include <string>
#include <sstream>

void * function(void * arg)
{
	Semaphore& s = *((Semaphore *) arg);
	for (int i = 0; i < 10; ++i)
	{
		--s;
		cout << "inthread: " << i << endl;
	}
	return NULL;
}

int main()
{
	pthread_t thread;

	int x;
	if (!(istringstream("2.5") >> x))
		cout << "ERROR!" << endl;
	else
		cout << x << endl;

	Semaphore s(0);

	pthread_create(&thread, NULL, function, (void *) &s);

	for (int i = 0; i < 10; ++i)
	{
		sleep(1);
		cout << "outthread: " << i << endl;
		++s;
	}

	sleep(1);

	int i = s;
	cout << "semaphore: " << i << endl;

	pthread_join(thread, NULL);
}
