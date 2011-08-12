/*
 * Filesystem.cpp
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#include "Filesystem.h"
#include "Error.h"
#include <sys/mman.h>


namespace webzavod {

MapFile::MapFile(int aFile, size_t aLength, off_t aOffset)
{
	void* map=mmap(NULL, aLength, PROT_WRITE, MAP_SHARED, aFile, aOffset);
	if (map==MAP_FAILED)
		throw MapFileErr();
}

MapFile::~MapFile()
{

}

void MapFile::Write(const Buffer & buffer)
{
	throw WriteFileErr();
}

}
