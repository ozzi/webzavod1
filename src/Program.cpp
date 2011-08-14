/*
 * Program.cpp
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#include "Program.h"
#include <algorithm>
#include <fcntl.h>
#include "Error.h"

namespace webzavod
{

Program::Program(const Params & aParams)
	: input(aParams.GetUrl()), output(aParams.GetOutput()),
	  threadsNumber(input.GetFileSize() ? aParams.GetThreadsCount() : 1),
	  barrier(threadsNumber), threads(threadsNumber, Thread(input.GetAddress(), output, &barrier, aParams.GetBufferSize()))
{}

class ThreadStartFunc
{
	size_t fileSize, threadsNumber, increment, counter;
public:
	ThreadStartFunc(size_t aFileSize, size_t aThreadsNumber)
		: fileSize(aFileSize), threadsNumber(aThreadsNumber), increment(fileSize/threadsNumber), counter(0) {}
	void operator() (Thread& thread)
	{
		thread.Start(counter*increment, counter!=threadsNumber-1 ? increment: fileSize-counter*increment);
		counter++;
	}
};

void Program::Work()
{
	std::for_each(threads.begin(), threads.end(), ThreadStartFunc(input.GetFileSize(), threadsNumber));
	barrier.Wait();
}

Program::~Program()
{
}

}


