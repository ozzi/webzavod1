/*
 * Program.cpp
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#include "Program.h"
#include <algorithm>

namespace webzavod {

Program::Program(const Params & aParams) : file(aParams.GetOutputFileName()), barrier(aParams.GetThreadsCount())
{
	threads.assign(aParams.GetThreadsCount(), Thread(Network(aParams.GetUrl()), &file, &barrier, aParams.GetBufferSize(), &section));
}

void Program::Work()
{
	std::for_each(threads.begin(), threads.end(), std::mem_fun_ref(&Thread::Start));
	barrier.Wait();
}

Program::~Program() {
}
}


