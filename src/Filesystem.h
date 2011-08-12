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

class MapFile {
public:
	MapFile(int aFile, size_t aLength, off_t aOffset);
	virtual ~MapFile();
	void Write(const Buffer & buffer);
};

}

#endif /* FILESYSTEM_H_ */
