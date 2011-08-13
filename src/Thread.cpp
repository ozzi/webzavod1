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
	return static_cast<Thread*>(aParam)->Func();
}

void* Thread::Func()
{
	void* res(worker.Main());
	barrier->Dec();
	return res;
}

Thread::~Thread()
{
}

}
