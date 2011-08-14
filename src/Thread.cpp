/*
 * Thread.cpp
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#include "Thread.h"
#include "Error.h"

namespace webzavod
{

void Thread::Start(size_t aOffset, size_t aBytes)
{
	offset=aOffset;
	bytes=aBytes;
	if (pthread_create(&id, NULL, EntryPoint, this))
		throw PthreadCreateErr();
}

void* Thread::EntryPoint(void * aParam)
{
	Thread* thread(static_cast<Thread*>(aParam));
	void* res(thread->Worker());
	thread->barrier->Dec();
	return res;
}

void* Thread::Worker()
{
	Http http(addr);
	http.SubmitRequest(PartialGETRequest(addr.GetResource(), offset, bytes));
	PartialGETResponse res(bufferSize);
	while (!http.ReceiveResponse(res))
		output.Write(res.GetRange(), res.GetDataSize(), res.GetData());
	return NULL;
}

Thread::~Thread()
{
}

}
