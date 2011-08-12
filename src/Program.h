/*
 * Program.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <vector>
#include "Params.h"
#include "Network.h"
#include "Filesystem.h"
#include "Barrier.h"
#include "Thread.h"
#include "CriticalSection.h"

namespace webzavod {

class Program
{
	OutputFile file;
	Barrier barrier;
	std::vector<Thread> threads;
	CriticalSection section;
public:
	Program(const Params & aParams);
	void Work();
	virtual ~Program();
};

}

#endif /* PROGRAM_H_ */
