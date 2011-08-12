/*
 * Error.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef ERROR_H_
#define ERROR_H_
#include <string>

namespace webzavod {

class Error {
public:
	virtual std::string Print() { return "Unknown error"; }
};

class UsageErr : public Error {
public:
	virtual std::string Print() { return "usage: blablabla"; }
};

class ThreadsErr : public Error {
public:
	virtual std::string Print() { return "threads: blablabla"; }
};

class BufferErr : public Error {
public:
	virtual std::string Print() { return "buffer: blablabla"; }
};

class PthreadCreateErr : public Error {
public:
	virtual std::string Print() { return "pthread_create error: blablabla"; }
};

}

#endif /* ERROR_H_ */
