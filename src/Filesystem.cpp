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

void OutputFile::Write(const Buffer & aBuffer, unsigned long aPosition)
{
	throw WriteFileErr();
}

}
