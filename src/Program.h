/*
 * Program.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef PROGRAM_H_
#define PROGRAM_H_

#include "Params.h"

namespace webzavod {

class Program {
public:
	Program();
	void Init(const Params & params);
	void Work();
	void Close();
	virtual ~Program();
};

}

#endif /* PROGRAM_H_ */
