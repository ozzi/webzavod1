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

namespace webzavod {

Program::Program(const Params & aParams) : barrier(aParams.GetThreadsCount())
{
	long fileSize(0);
	output=open(aParams.GetOutputFileName().c_str(), O_WRONLY | O_CREAT | O_EXCL);
	if (!output)
		throw CreateFileErr();
	for (unsigned threadNumber(0); threadNumber<aParams.GetThreadsCount(); threadNumber++)
	{
		long offset();
		long size();
		threads.push_back(Thread(Worker(offset, size), &barrier));
	}
}

void Program::Work()
{
	std::for_each(threads.begin(), threads.end(), std::mem_fun_ref(&Thread::Start));
	barrier.Wait();
}

Program::~Program()
{
	close(output);
}
}


