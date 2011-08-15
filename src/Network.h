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
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace webzavod {

class IP
{
	unsigned long addr;
public:
	IP(){}
	IP(const std::string& aAddr)
	{
		SetAddr(aAddr);
	}
	~IP(){}
	void SetAddr(const std::string& aAddr);
	const in_addr Address() const
	{
		in_addr inaddr;
		inaddr.s_addr=addr;
		return inaddr;
	}
};

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
	const char* GetData() const { return 0; }
	const size_t Size() const { return 0; }
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
	std::vector<char> header;
	std::vector<char> data;
	size_t recvSize;

	void GetHeader();
public:
	Response(const size_t aBufferSize=4096) : data(aBufferSize, 0), recvSize(0) {}
	virtual ~Response(){}
	char* GetData() { return &data[0]; }
	const size_t GetMaxSize() const { return data.size(); }
	const size_t GetRecvSize() const { return recvSize; }
	const size_t GetHeaderSize() const { return header.size(); }
	void SetRecvSize(size_t bytes)
	{
		recvSize=bytes;
		GetHeader();
	}
};

class ResponseWithLength: public Response
{
	size_t contentLength;
public:
	ResponseWithLength(const size_t aBufferSize=4096) : Response(aBufferSize), contentLength(0) {}
	const size_t GetContentLength() const { return contentLength; }
};

class HEADResponse: public ResponseWithLength
{
public:
	HEADResponse(const size_t aBufferSize=4096) : ResponseWithLength(aBufferSize){}
	virtual ~HEADResponse(){}
};

class GETResponse: public ResponseWithLength
{
public:
	GETResponse(const size_t aBufferSize=4096) : ResponseWithLength(aBufferSize) {}
	virtual ~GETResponse(){}
};

class PartialGETResponse: public GETResponse
{
	size_t range;
public:
	PartialGETResponse(const size_t aBufferSize=4096) : GETResponse(aBufferSize){}
	virtual ~PartialGETResponse(){}
	const size_t GetResourceRange() const { return range; }
};

class Socket
{
	int id;
public:
	Socket();
	~Socket();
	int Connect(const IP& ip);
	int Send(const char* aData, const size_t aSize);
	int Receive(char* aData, const size_t aSize);
};

class Http
{
	std::string base;
	Socket socket;
public:
	Http(const Address& aAddr) : base(aAddr.GetBase()) {}
	void SubmitAllRequest(const Request& request);
	bool ReceiveResponse(Response& response);
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
