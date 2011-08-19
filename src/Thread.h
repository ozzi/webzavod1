//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================

#ifndef THREAD_H_
#define THREAD_H_

#include "Barrier.h"
#include "Network.h"
#include "Filesystem.h"
#include <pthread.h>

namespace webzavod {

class Thread
{
	//класс реализует потоковую загрузку
	pthread_t id;
	UrlHttp addr;
	OutputFile* output;
	Barrier* barrier;
	size_t offset;//начиная с этого смещения на сервере поток загрузит указанное количество байт
	size_t total;//количество байт, которое необходимо загрузить
	size_t received;//содержит текущее количество загруженных байт
	bool error;//поток столкнулся с ошибкой во время работы

	void* Worker();//тело потока, здесь происходит закачка файла
	static void* EntryPoint(void* aParam);//точка входа для pthread_create
public:
	Thread(const UrlHttp& aAddr, OutputFile* aFile, Barrier* aBarrier)
		: id(0),
		  addr(aAddr),
		  output(aFile),
		  barrier(aBarrier),
		  offset(0),
		  total(0),
		  received(0),
		  error(false) {}
	virtual ~Thread();
	void Start(size_t aOffset, size_t aBytes);
	const size_t GetBytesReceived() const { return received; }
	const bool GetErrorState() const { return error; }
};

}

#endif /* THREAD_H_ */
