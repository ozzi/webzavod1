/*
 * Worker.h
 *
 *  Created on: 13.08.2011
 *      Author: outz
 */

#ifndef WORKER_H_
#define WORKER_H_

namespace webzavod {

class Worker {
public:
	Worker();
	virtual ~Worker();
	void* Main() { return 0; }
};

}

#endif /* WORKER_H_ */
