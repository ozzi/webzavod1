/*
 * CriticalSection.h
 *
 *  Created on: 12.08.2011
 *      Author: outz
 */

#ifndef CRITICALSECTION_H_
#define CRITICALSECTION_H_

#include <pthread.h>

namespace webzavod {

class CriticalSection {
	pthread_mutex_t mutex;
public:
	CriticalSection();
	virtual ~CriticalSection();
	void Enter();
	void Leave();
};

}

#endif /* CRITICALSECTION_H_ */
