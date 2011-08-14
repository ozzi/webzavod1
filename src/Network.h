/*
 * Network.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <string>
#include <vector>

namespace webzavod {

class IP
{};

class Address
{
	IP ip;
	std::string base;
	std::string resource;
public:
	Address(const std::string& aUrl);
	~Address(){};
	const std::string& GetBase() const { return base; }
	const std::string& GetResource() const { return resource; }
	const IP& GetIp() const { return ip; }
};

class Request
{
public:
	Request(){}
	virtual ~Request(){}
};

class HEADRequest : public Request
{
	const std::string& resource;
public:
	HEADRequest(const std::string& aResource) : resource(aResource) {}
	virtual ~HEADRequest(){}
};

class GETRequest : public Request
{
	const std::string& resource;
public:
	GETRequest(const std::string& aResource) : resource(aResource) {}
	virtual ~GETRequest(){}
};

class PartialGETRequest : public GETRequest
{
public:
	PartialGETRequest(const std::string& aResource, const size_t aRange , const size_t aBytes) : GETRequest(aResource) {}
	virtual ~PartialGETRequest(){}
};


class Response
{
public:
	Response(){}
	virtual ~Response(){}
};

class HEADResponse: public Response
{
	size_t length;
public:
	HEADResponse(){}
	virtual ~HEADResponse(){}
	const size_t GetContentLength() const { return length; }
};

class GETResponse: public Response
{
	std::vector<char> data;
public:
	GETResponse(const size_t aBufferSize) : data(aBufferSize, 0) {}
	virtual ~GETResponse(){}
	const void* GetData() const { return &data[0]; }
	const size_t GetDataSize() const { return data.size(); }
};

class PartialGETResponse: public GETResponse
{
	size_t range;
public:
	PartialGETResponse(const size_t bufferSize) : GETResponse(bufferSize){}
	virtual ~PartialGETResponse(){}
	const size_t GetRange() const { return range; }
};

class Socket
{
public:
	Socket(const IP& aIp){};
	~Socket(){};
	void Send(const Request& request);
	void Receive(Response& responce);
};

class Http
{
	std::string base;
	Socket socket;
public:
	Http(const Address& aAddr) : base(aAddr.GetBase()), socket(aAddr.GetIp()) {}
	bool SubmitRequest(const Request& request);
	bool ReceiveResponse(Response& responce);
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
