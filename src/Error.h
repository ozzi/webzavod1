//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================
//набор классов оболочек для вывода ошибок

#ifndef ERROR_H_
#define ERROR_H_
#include <sstream>
#include <string>
#include <cstring>
#include <cerrno>
#include <netdb.h>

namespace webzavod {

class Error {
public:
	virtual std::string Print() { return "Unknown error"; }
};

//CommandLineErrors
class ParseHelpErr : public Error {
public:
	virtual std::string Print()
	{
		std::stringstream ss;
		ss<<"-t, --threads-count=number set the number of threads downloads.\n";
		ss<<"-o, --output-file=filename set saved file name. default save as resource name.\n";
		ss<<"-r, --refresh-time=time    set time update interval on the progress of downloads.\n";
		ss<<"-h, --help                 print this help.\n";
		return ss.str();
	}
};

class ParseUsageErr : public Error {
public:
	virtual std::string Print()
	{
		return "Usage: webzavod1 [option]... [url]\nTry `webzavod1 --help' for more options\n";
	}
};

class ParseThreadsNumberErr : public Error {
public:
	virtual std::string Print() { return "Parse command line: incorrect number of threads\n"; }
};

class ParseUpdateIntervalErr: public Error {
public:
	virtual std::string Print() { return "Parse command line: incorrect update interval\n"; }
};

class PthreadCreateErr : public Error {
	int err;
public:
	PthreadCreateErr(int aErr) : err(aErr) {}
	virtual std::string Print() {
		std::stringstream ss;
		ss<<"pthread_create() error; error value="<<err<<"\n";
		return ss.str();
	}
};

class WrongleUrlErr: public Error {
public:
	virtual std::string Print() { return "wrongle url error\n"; }
};

class UnsuccessfulServerResponseErr: public Error {
	std::string response;
public:
	UnsuccessfulServerResponseErr(const std::string& aResponse)
		: response(aResponse) {}
	virtual std::string Print() {
		std::stringstream ss;
		ss<<"unsuccessful server response\n";
		ss<<response<<"\n";
		return ss.str();
	}
};

class GetAddrInfoErr: public Error {
	int err;
public:
	GetAddrInfoErr(int aErr) : err(aErr) {}
	virtual std::string Print()
	{
		std::stringstream ss;
		ss<<"getaddrinfo() error: "<<gai_strerror(err)<<"\n";
		return ss.str();
	}
};

class PthreadSpinInitErr: public Error {
	int err;
public:
	PthreadSpinInitErr(int aErr) : err(aErr) {}
	virtual std::string Print() {
		std::stringstream ss;
		ss<<"pthread_spin_init() error; error number: "<<err<<"\n";
		return ss.str();
	}
};

class PthreadCondInitErr: public Error {
	int err;
public:
	PthreadCondInitErr(int aErr) : err(aErr) {}
	virtual std::string Print() {
		std::stringstream ss;
		ss<<"pthread_cond_init() error; error number: "<<err<<"\n";
		return ss.str();
	}
};

class CreateSocketErr: public Error {
public:
	virtual std::string Print() {
		std::stringstream ss;
		ss<<"socket() error; "<<strerror(errno)<<std::endl;
		return ss.str();
	}
};

class SocketConnectErr: public Error {
public:
	virtual std::string Print() {
		std::stringstream ss;
		ss<<"connect() error; "<<strerror(errno)<<std::endl;
		return ss.str();
	}
};

class SendSocketErr: public Error {
public:
	virtual std::string Print() {
		std::stringstream ss;
		ss<<"send() error; "<<strerror(errno)<<std::endl;
		return ss.str();
	}
};

class RecvSocketErr: public Error {
public:
	virtual std::string Print() {
		std::stringstream ss;
		ss<<"recv() error; "<<strerror(errno)<<std::endl;
		return ss.str();
	}
};

class CreateFileErr: public Error {
	std::string name;
public:
	CreateFileErr(const std::string aName) : name(aName) {}
	virtual std::string Print()
	{
		std::stringstream ss;
		ss<<"open() error; name: "<<name<<" strerror(): "<<strerror(errno)<<std::endl;
		return ss.str();
	}
};

class WriteFileErr: public Error {
	std::string fileName;
	size_t bytes, pos;
public:
	WriteFileErr(const std::string& aFileName, size_t aBytes, size_t aPosition)
		: fileName(aFileName), bytes(aBytes), pos(aPosition) {}
	virtual std::string Print()
	{
		std::stringstream ss;
		ss<<"pwrite() error; file name: "<<fileName<<"; bytes: "<<bytes<<"; position: "<<pos;
		ss<<"; strerror(): "<<strerror(errno)<<std::endl;
		return ss.str();
	}
};

class ThreadWorkerErr: public Error {
public:
	virtual std::string Print()
	{
		return "Thread::Worker() error\n";
	}
};

class TransactionCompletedIncorrectlyErr: public Error {
	size_t id, recv, total;
public:
	TransactionCompletedIncorrectlyErr(int aId, size_t aRecv, size_t aBytes)
	: id(aId), recv(aRecv), total(aBytes) {}
	virtual std::string Print()
	{
		std::stringstream ss;
		ss<<"[thread "<<id<<"] transaction is completed incorrectly, received "<<recv;
		ss<<" bytes of "<<total<<" total\n";
		return ss.str();
	}
};

}

#endif /* ERROR_H_ */
