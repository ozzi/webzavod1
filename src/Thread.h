/*
 * Thread.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "Barrier.h"
#include "Network.h"
#include <pthread.h>

namespace webzavod {

class Thread
{
	pthread_t id;
	Barrier* barrier;

	void* Worker();
	static void* EntryPoint(void* aParam);
public:
	Thread() {}
	void Start();
	virtual ~Thread();
};

}

#endif /* THREAD_H_ */
