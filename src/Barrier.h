/*
 * Barrier.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef BARRIER_H_
#define BARRIER_H_

namespace webzavod {

class Barrier {
	unsigned threadCount;
public:
	Barrier(unsigned aThreadCount) : threadCount(aThreadCount) {}
	void Wait();
	virtual ~Barrier();
	void Dec();
};

}

#endif /* BARRIER_H_ */
