/*
 * Thread.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "Worker.h"
#include "Barrier.h"
#include <pthread.h>

namespace webzavod {

class Thread
{
	pthread_t id;
	Worker worker;
	Barrier* barrier;

	void* Func();
	static void * EntryPoint(void * aParam);
public:
	Thread(const Worker& aWorker, Barrier* aBarrier) : worker (aWorker), barrier(aBarrier) {}
	void Start();
	virtual ~Thread();
};

}

#endif /* THREAD_H_ */
