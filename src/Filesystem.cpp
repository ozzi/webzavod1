/*
 * Filesystem.cpp
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#include "Filesystem.h"
#include "Error.h"

namespace webzavod {

OutputFile::OutputFile(const std::string& aName)
{
	file=open(aName.c_str(), O_WRONLY | O_CREAT | O_EXCL);
	if (file==-1)
		throw CreateFileErr();
}

OutputFile::~OutputFile()
{
	close(file);
}

void OutputFile::Write(const void* aData, size_t aBytes, size_t aPosition)
{
	if (pwrite(file, aData, aBytes, aPosition)==-1)
		throw WriteFileErr();
}

}
