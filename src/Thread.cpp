/*
 * Thread.cpp
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#include "Thread.h"
#include "Error.h"
#include <iostream>

namespace webzavod
{
Thread::Thread(Network * aNetwork, Filesystem * aFile, Barrier * aBarrier) :
		pNetwork(aNetwork), pFile(aFile), pBarrier(aBarrier) {}

void Thread::Start()
{
	if (pthread_create(&id, NULL, EntryPoint, this))
		throw PthreadCreateErr();
	std::cout<<"thread with id "<<id<<" started\n";
}

void * Thread::EntryPoint(void * aArg)
{
	Thread * pThread(static_cast<Thread*>(aArg));
	pThread->Func();
	return NULL;
}

void Thread::Func()
{
	std::cout<<"thread with id "<<id<<" finished\n";
	pBarrier->Dec();
}

Thread::~Thread()
{
}

}
