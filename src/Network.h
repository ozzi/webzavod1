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
protected:
	std::string str;
public:
	Request();
	virtual ~Request(){}
	const char* GetData() const { return 0; }
	const size_t Size() const { return 0; }
};

class HEADRequest : public Request
{
public:
	HEADRequest(const std::string& aResource);
	virtual ~HEADRequest(){}
};

class GETRequest : public Request
{
public:
	GETRequest(const std::string& aResource);
	virtual ~GETRequest(){}
};

class PartialGETRequest : public GETRequest
{
public:
	PartialGETRequest(const std::string& aResource, const size_t aRange , const size_t aBytes);
	virtual ~PartialGETRequest(){}
};

class Response
{
	std::vector<char> buffer;
	std::vector<char> header;
	std::vector<char> data;
	size_t recvSize;
	bool headerReceived;

	void CalculateHeader();
public:
	Response(const size_t aBufferSize=4096) : buffer(aBufferSize, 0), recvSize(0), headerReceived(false) {}
	virtual ~Response(){}
	char* GetBuffer() { return &buffer[0]; }
	char* GetData() { return &data[0]; }
	const size_t GetRecvSize() const { return recvSize; }
	const size_t GetBufferSize() const { return buffer.size(); }
	const size_t GetDataSize() const { return data.size(); }
	void SetRecvSize(size_t bytes);
	const std::string GetLabelValue(const std::string& label) const;
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
