/*
 * Program.cpp
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#include "Program.h"
#include <algorithm>

namespace webzavod {

Program::Program(const Params & aParams) : net(aParams.GetUrl()), file(aParams.GetOutputFileName()), barrier(aParams.GetThreadsCount())
{
	threads.assign(aParams.GetThreadsCount(), Thread(&net, &file, &barrier));
}

void Program::Work()
{
	std::for_each(threads.begin(), threads.end(), std::mem_fun_ref(&Thread::Start));
	barrier.Wait();
}

Program::~Program() {
}
}


