/*
 * Thread.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef THREAD_H_
#define THREAD_H_

#include "Network.h"
#include "Filesystem.h"
#include "Barrier.h"

namespace webzavod {

class Thread {
public:
	Thread(const Network & aNetwork, const Filesystem & aFile, const Barrier & aBarrier);
	void Start();
	virtual ~Thread();
};

}

#endif /* THREAD_H_ */
