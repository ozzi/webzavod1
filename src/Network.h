/*
 * Network.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <string>
#include "Buffer.h"

namespace webzavod {

class Address
{
	std::string base;
	std::string resource;
public:
	Address(const std::string& aUrl);
	~Address(){};
};

class Request
{
public:
	Request(){}
	virtual ~Request(){}
};

class HeadRequest : public Request
{
public:
	HeadRequest(const Address& addr);

};

class Response
{
	size_t contentLength;
public:
	const size_t GetContentLength() const { return contentLength; }
};

class Socket
{
public:
	Socket(const Address& aAddr){};
	~Socket(){};
	void Send(const Request& request);
	void Receive(Response& responce);
};

class Http
{
	Address addr;
	Socket socket;
public:
	Http(const Address& aAddr) : addr (aAddr), socket(aAddr) {}
	const Response Head();
};

class InputInfo
{
	Address addr;
	size_t fileSize;
public:
	InputInfo(const std::string& aAddr);
	const size_t GetFileSize() const  { return fileSize; }
	const Address& GetAddress() const { return addr; }
};

}

#endif /* NETWORK_H_ */
