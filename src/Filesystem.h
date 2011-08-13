/*
 * Filesystem.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <string>
#include <fcntl.h>
#include "Buffer.h"

namespace webzavod {

class OutputFile {
	int file;
public:
	OutputFile(const std::string& aName);
	virtual ~OutputFile();
	void Write(const Buffer & aBuffer, unsigned long aPosition);
};

}

#endif /* FILESYSTEM_H_ */
