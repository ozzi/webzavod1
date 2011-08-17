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
	virtual std::string Print() { return "usage: webzavod1 options url"; }
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

class WrongleUrlErr: public Error {
public:
	virtual std::string Print() { return "wrongle url error: blablabla"; }
};

class ResourceInfoErr: public Error {
public:
	virtual std::string Print() { return "ResourceInfo"; }
};

class GetAddrInfoErr: public Error {
	int err;
public:
	GetAddrInfoErr(int aErr) : err(aErr) {}
	virtual std::string Print() { return "GetAddrInfoErr"; }
};

class SetAddrErr: public Error {
public:
	virtual std::string Print() { return "set addr err"; }
};

class CreateSocketErr: public Error {
public:
	virtual std::string Print() { return "inet_aton error: blablabla"; }
};

class SendSocketErr: public Error {
public:
	virtual std::string Print() { return "send socket error: blablabla"; }
};

class RecvSocketErr: public Error {
public:
	virtual std::string Print() { return "recv socket error: blablabla"; }
};

class CreateFileErr: public Error {
public:
	virtual std::string Print() { return "create file error: blablabla"; }
};

class WriteFileErr: public Error {
public:
	virtual std::string Print() { return "write file error: blablabla"; }
};

class MapFileErr: public Error {
public:
	virtual std::string Print() { return "map file error: blablabla"; }
};

}

#endif /* ERROR_H_ */
