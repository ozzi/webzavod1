/*
 * Network.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <string>
#include <sstream>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace webzavod {

class IP {
	unsigned long addr;
public:
	IP() {
	}
	IP(const std::string& aAddr) {
		SetAddr(aAddr);
	}
	~IP() {
	}
	void SetAddr(const std::string& aAddr);
	const in_addr Address() const {
		in_addr inaddr;
		inaddr.s_addr = addr;
		return inaddr;
	}
};

class Address {
	IP ip;
	std::string base;
	std::string resource;
public:
	Address(const std::string& aUrl);
	~Address() {
	}
	;
	const std::string& GetBase() const {
		return base;
	}
	const std::string& GetResource() const {
		return resource;
	}
	const IP& GetIp() const {
		return ip;
	}
};

class Request {
	std::string uri;
	std::string version;
protected:
	const std::string _get(const char* method) const
	{
		std::stringstream get;
		get << method << " " << uri << " HTTP/" << version << "\r\n";
		return get.str();
	}
public:
	Request(const std::string& aUri) : uri(aUri), version("1.0") {}
	virtual ~Request() {}
	virtual const std::string Get() const { return _get("OPTIONS"); }
};

class HEADRequest: public Request {
public:
	HEADRequest(const std::string& aUri) :
		Request(aUri) {}
	virtual ~HEADRequest() {}
	virtual const std::string Get() const { return _get("HEAD"); }
};

class GETRequest: public Request {
public:
	GETRequest(const std::string& aUri) : Request(aUri) {}
	virtual ~GETRequest() {}
	virtual const std::string Get() const { return _get("GET"); }
};

class PartialGETRequest: public GETRequest {
	size_t beginRange, endRange;
public:
	PartialGETRequest(const std::string& aUri, const size_t aRange, const size_t aBytes)
		: GETRequest(aUri), beginRange(aRange), endRange(aRange + aBytes) {}
	virtual ~PartialGETRequest() {}
	virtual const std::string Get() const {
		std::stringstream get(GETRequest::Get());
		get << "Range: bytes=" << beginRange << "-" << endRange << "\r\n";
		return get.str();
	}
};

class Response {
	std::vector<char> buffer;
	std::vector<char> header;
	std::vector<char> data;
	size_t recvSize;
	bool headerReceived;

	void CalculateHeader();
public:
	Response(const size_t aBufferSize = 4096) :
		buffer(aBufferSize, 0), recvSize(0), headerReceived(false) {
	}
	virtual ~Response() {
	}
	char* GetBuffer() {
		return &buffer[0];
	}
	char* GetData() {
		return &data[0];
	}
	const size_t GetRecvSize() const {
		return recvSize;
	}
	const size_t GetBufferSize() const {
		return buffer.size();
	}
	const size_t GetDataSize() const {
		return data.size();
	}
	void SetRecvSize(size_t bytes);
	const std::string GetLabelValue(const std::string& label) const;
};

class Socket {
	int id;
public:
	Socket();
	~Socket();
	int Connect(const IP& ip);
	int Send(const char* aData, const size_t aSize);
	int Receive(char* aData, const size_t aSize);
};

class Http {
	IP ip;
	std::string base;
	Socket socket;
public:
	Http(const Address& aAddr) : ip(aAddr.GetIp()), base(aAddr.GetBase()) {}
	void SubmitAllRequest(const Request& request);
	bool ReceiveResponse(Response& response);
};

class InputInfo {
	Address addr;
	size_t fileSize;
public:
	InputInfo(const std::string& aAddr);
	const size_t GetFileSize() const {
		return fileSize;
	}
	const Address& GetAddress() const {
		return addr;
	}
};

}

#endif /* NETWORK_H_ */
