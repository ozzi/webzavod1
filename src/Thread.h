/*
 * Thread.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "Network.h"
#include "Filesystem.h"
#include "Barrier.h"
#include <pthread.h>

namespace webzavod {

class Thread {
	pthread_t id;
	Network * pNetwork;
	Filesystem * pFile;
	Barrier * pBarrier;
	static void * EntryPoint(void * aArg);
	void Func();
public:
	Thread(Network * pNetwork, Filesystem * pFile, Barrier * pBarrier);
	void Start();
	virtual ~Thread();
};

}

#endif /* THREAD_H_ */
