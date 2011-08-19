//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "Params.h"
#include "Network.h"
#include "Filesystem.h"
#include "Barrier.h"
#include "Thread.h"
#include <vector>

namespace webzavod {

class Program
{
	//класс реализующий многопоточную закачку
	ResourceInfoSession input;//класс, проверяющий наличие ресурса на сервере и содержащий сведения о ресурсе
	OutputFile output;//выходной файл
	size_t threadsNumber;//количество потоков
	Barrier barrier;//точка синхронизации
	std::vector<Thread> threads;//потоки
public:
	Program(const Params & aParams);
	void Work();//реализация работы класса
	virtual ~Program();
};

}

#endif /* PROGRAM_H_ */
