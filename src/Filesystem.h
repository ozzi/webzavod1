/*
 * Filesystem.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <stdio.h>
#include <string>
#include "Buffer.h"

namespace webzavod {

class OutputFile {
	FILE *pFile;
public:
	OutputFile(const std::string & aName);
	virtual ~OutputFile();
	void Put(const Buffer & buffer);
};

}

#endif /* FILESYSTEM_H_ */
