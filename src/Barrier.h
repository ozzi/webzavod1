//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================

#ifndef BARRIER_H_
#define BARRIER_H_

#include <pthread.h>
#include <sys/types.h>

namespace webzavod {

class Barrier {
	//смысл данного класса реализовать pthread_multijoin более менее корректно
	//то есть с его помощью в некоей точке ждется завершение указанного количества потоков
	unsigned threadCount;//счетчик количества работающих потоков
	pthread_spinlock_t spinlock;//спинлок, используется при уменьшении счетчика
	pthread_cond_t cond;//условная переменная, используется при ожидании
	pthread_mutex_t mutex;//условно используется
	time_t sec;//хранит временной интервал, используемый в TimedWait
public:
	Barrier(unsigned aThreadCount, const time_t aSec);
	void TimedWait();//ждем наступления события либо наступления временного интервала
	virtual ~Barrier();
	void Dec();//уменьшаем счетчик
	int Count() { return threadCount; }
};

}

#endif /* BARRIER_H_ */
