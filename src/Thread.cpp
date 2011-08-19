//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================

#include "Thread.h"
#include "Error.h"

namespace webzavod
{

void Thread::Start(size_t aOffset, size_t aBytes)
{
	offset=aOffset;
	total=aBytes;
	error=false;
	received=0;
	int err(pthread_create(&id, NULL, EntryPoint, this));
	if (err)
		throw PthreadCreateErr(err);
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
	try {
		SessionHttp http(addr);
		http.SubmitAllRequest(PartialGETRequestHttp(addr, offset, total));
		ResponseHttp res;
		long position(offset);
		std::cout<<"[thread "<<id<<"] Receive data begins...\n";
		while (!http.ReceiveResponse(res))
		{
			output->Write(res.GetMsgBody(), res.GetMsgBodySize(), position);
			position+=res.GetMsgBodySize();
			received+=res.GetMsgBodySize();
		}
		if (received!=total)
			throw TransactionCompletedIncorrectlyErr(id, received, total);
		std::cout<<"[thread "<<id<<"] Receive data is successfully completed.\n";
	}
	catch (webzavod::Error &err)
	{
		std::cout<<err.Print();
		error=true;
	}
	id=0;
	return NULL;
}

Thread::~Thread()
{
	if (id)
		pthread_cancel(id);
}

}
