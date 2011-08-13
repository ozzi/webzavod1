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

void Thread::Start()
{
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
	Address* addr(NULL);//!!!!!!!!!!!!!!!
	Http http(*addr);
	return NULL;
}

Thread::~Thread()
{
}

}
