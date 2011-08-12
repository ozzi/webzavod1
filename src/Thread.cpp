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

void Thread::Start()
{
	if (pthread_create(&id, NULL, EntryPoint, this))
		throw PthreadCreateErr();
	std::cout<<"thread with id "<<id<<" started\n";
}

void* Thread::EntryPoint(void * aParam)
{
	return static_cast<Thread*>(aParam)->Worker();
}

void* Thread::Worker()
{
	void * res(NULL);
	for (net.Init(); net.Connected(); net.Get(buffer))
		map.Write(buffer);
	pBarrier->Dec();
	std::cout<<"thread with id "<<id<<" finished\n";
	return res;
}

Thread::~Thread()
{
}

}
