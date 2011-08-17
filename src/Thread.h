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
#include "Filesystem.h"
#include <pthread.h>

namespace webzavod {

class Thread
{
	pthread_t id;
	UrlHttp addr;
	OutputFile output;
	Barrier* barrier;
	size_t bufferSize;
	size_t offset;
	size_t bytes;

	void* Worker();
	static void* EntryPoint(void* aParam);
public:
	Thread(const UrlHttp& aAddr, const OutputFile& aFile, Barrier* aBarrier, size_t aBlockSize)
		: id(0), addr(aAddr), output(aFile), barrier(aBarrier), bufferSize(aBlockSize), offset(0), bytes(0) {}
	void Start(size_t aOffset, size_t aBytes);
	virtual ~Thread();
};

}

#endif /* THREAD_H_ */
