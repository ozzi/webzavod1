//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================

#include "Barrier.h"
#include "Error.h"

namespace webzavod {

Barrier::Barrier(unsigned aThreadCount, const time_t aSec)
	: threadCount(aThreadCount)
{
	sec=aSec;
	int err(pthread_spin_init(&spinlock,PTHREAD_PROCESS_PRIVATE));
	if (err)
		throw PthreadSpinInitErr(err);
	err=pthread_cond_init(&cond, NULL);
	if (err)
		throw PthreadCondInitErr(err);
	err=pthread_mutex_init(&mutex, NULL);
}

Barrier::~Barrier() {
	pthread_spin_destroy(&spinlock);
	pthread_cond_destroy(&cond);
	pthread_mutex_destroy(&mutex);
}

void Barrier::TimedWait()
{
	timespec abstime;
	clock_gettime(CLOCK_REALTIME_COARSE, &abstime);
	abstime.tv_sec+=sec;
	pthread_mutex_lock(&mutex);
	pthread_cond_timedwait(&cond, &mutex, &abstime);
	pthread_mutex_unlock(&mutex);
}

void Barrier::Dec()
{
	pthread_spin_lock(&spinlock);
	--threadCount;
	pthread_cond_signal(&cond);
	pthread_spin_unlock(&spinlock);
}

}
