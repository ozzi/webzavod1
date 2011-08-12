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
#include "Buffer.h"
#include <pthread.h>

namespace webzavod {

class Thread
{
	pthread_t id;
	Network net;
	MapFile map;
	Barrier * pBarrier;
	Buffer buffer;

	static void * EntryPoint(void * aParam);
	void * Worker();
public:
	Thread(const Source & aSource, int aFile, Barrier * aBarrier, const unsigned aBufferSize)
		: net(aSource), map(aFile, 0, 0), pBarrier(aBarrier), buffer(aBufferSize) {}
	void Start();
	virtual ~Thread();
};

}

#endif /* THREAD_H_ */
