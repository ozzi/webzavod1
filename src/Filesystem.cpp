/*
 * Filesystem.cpp
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#include "Filesystem.h"
#include "Error.h"

namespace webzavod {

OutputFile::OutputFile(const std::string & aName)
{
	pFile=fopen(aName.c_str(), "wb");
	if (!pFile)
		throw OpenFileErr();
}

OutputFile::~OutputFile()
{
	if (pFile)
		fclose(pFile);
}

void OutputFile::Put(const Buffer & buffer)
{
	if (!pFile)
		throw PutFileErr();
	fseek(pFile, buffer.Pos(), SEEK_SET);
	if (!fwrite(buffer.Data(), buffer.Size(), 1, pFile))
		throw WriteFileErr();
}

}
