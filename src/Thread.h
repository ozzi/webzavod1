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
#include "CriticalSection.h"
#include <pthread.h>

namespace webzavod {

class Thread
{
	pthread_t id;
	Network net;
	OutputFile * pFile;
	Barrier * pBarrier;
	Buffer buffer;
	CriticalSection * pSection;

	static void * EntryPoint(void * aParam);
	void * Worker();
public:
	Thread(const Network & aNetwork, OutputFile * aFile, Barrier * aBarrier, const unsigned aBufferSize, CriticalSection * aSection)
		: net(aNetwork), pFile(aFile), pBarrier(aBarrier), buffer(aBufferSize), pSection(aSection) {}
	void Start();
	virtual ~Thread();
};

}

#endif /* THREAD_H_ */
