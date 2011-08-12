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
#include "Network.h"

namespace webzavod {

Program::Program(const Params & aParams) : barrier(aParams.GetThreadsCount())
{
	output=open(aParams.GetOutputFileName().c_str(), O_WRONLY | O_CREAT | O_EXCL);
	if (!output)
		throw CreateFileErr();
	threads.assign(aParams.GetThreadsCount(),
		Thread(Source(aParams.GetUrl()), output, &barrier, aParams.GetBufferSize()));
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


