//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <string>
#include <fcntl.h>

namespace webzavod {

class OutputFile {
	//простенький класс, реализующий создание файла для вывода в него информации
	int file;
	std::string fileName;
public:
	OutputFile(const std::string& aName, const size_t aSize);
	virtual ~OutputFile();
	void Write(const void* aData, size_t aBytes, size_t aPosition);//запись в файл с помощью pwrite является thread safe
	const std::string& GetFileName() const { return fileName; }
};

}

#endif /* FILESYSTEM_H_ */
