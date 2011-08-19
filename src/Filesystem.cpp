//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================

#include "Filesystem.h"
#include "Error.h"

namespace webzavod {

OutputFile::OutputFile(const std::string& aName, const size_t aSize)
	: fileName(aName)
{
	file=open(fileName.c_str(), O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
	if (file==-1)
		throw CreateFileErr(fileName);
}

OutputFile::~OutputFile()
{
	close(file);
}

void OutputFile::Write(const void* aData, size_t aBytes, size_t aPosition)
{
	int bytes(pwrite(file, aData, aBytes, aPosition));
	if (bytes==-1)
		throw WriteFileErr(fileName, aBytes, aPosition);
}

}
