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
}

OutputFile::~OutputFile()
{

}

void OutputFile::Write(size_t aPosition, size_t aBytes, const void* aData)
{
	throw WriteFileErr();
}

}
