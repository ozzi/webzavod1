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
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace webzavod {

class InetSockAddr {
	sockaddr_in address;
public:
	InetSockAddr(){}
	InetSockAddr(const std::string& aAddr, const std::string& aPort);
	~InetSockAddr(){}
	void SetIpv4Addr(const std::string& aAddr, const std::string& aPort);
	const sockaddr_in& GetIpv4Addr() const;
};

class UrlHttp {
	InetSockAddr address;
	std::string host;
	std::string uri;
public:
	UrlHttp(){}
	UrlHttp(const std::string& aUrl);
	~UrlHttp() {}
	const std::string& GetHost() const { return host; }
	const std::string& GetUri() const { return uri; }
	const InetSockAddr& GetAddress() const { return address; }
};

class RequestHttp {
	std::string version;
	std::string uri;
	std::string host;
protected:
	const std::string _get(const char* method) const
	{
		std::stringstream get;
		get << method << " " << uri << " HTTP/" << version << "\n";
		get << "Host: " << host << "\n";
		get << "Accept: */*\n"
		return get.str();
	}
public:
	RequestHttp() : version("1.0") {}
	RequestHttp(const UrlHttp& aUrl) : version("1.0"), uri(aUrl.GetUri()), host(aUrl.GetHost())  {}
	virtual ~RequestHttp() {}
	void Init(const UrlHttp& aUrl) { uri.assign(aUrl.GetUri()); host.assign(aUrl.GetHost()); }
	virtual const std::string Get() const { return _get("OPTIONS")+"\n"; }
};

class HEADRequestHttp: public RequestHttp {
public:
	HEADRequestHttp(){}
	HEADRequestHttp(const UrlHttp& aUrl) : RequestHttp(aUrl) {}
	virtual ~HEADRequestHttp() {}
	virtual const std::string Get() const { return _get("HEAD")+"\n"; }
};

class GETRequestHttp: public RequestHttp {
public:
	GETRequestHttp();
	GETRequestHttp(const UrlHttp& aUrl) : RequestHttp(aUrl) {}
	virtual ~GETRequestHttp() {}
	virtual const std::string Get() const { return _get("GET")+"\n"; }
};

class PartialGETRequestHttp: public GETRequestHttp {
	size_t beginRange, endRange;
public:
	PartialGETRequestHttp(){}
	PartialGETRequestHttp(const UrlHttp& aUrl, const size_t aRange, const size_t aBytes)
		: GETRequestHttp(aUrl), beginRange(aRange), endRange(aRange + aBytes) {}
	virtual ~PartialGETRequestHttp() {}
	virtual const std::string Get() const
	{
		std::stringstream get(GETRequestHttp::Get());
		get << "Range: bytes=" << beginRange << "-" << endRange << "\n\n";
		return get.str();
	}
};

class ResponseHttp {
	std::vector<char> message;
	std::string startLine;
	std::string headersLine;
	std::map<std::string, std::string> headers;
	std::vector<char> msgBody;
	size_t messageSize;
	std::string version;
	bool startLineProcessed;
	bool headersLineProcessed;
	bool recvAllMsgBody;
	int statusCode;
	UrlHttp location;
	std::vector<char>::iterator beginParse;
	void GetStartLine();
	void GetHeadersLine();
	void ParseHeaders();
	void GetMessageBody();

public:
	ResponseHttp(const size_t aBufferSize = 4096) :
		message(aBufferSize, 0), messageSize(0), startLineProcessed(false), headersLineProcessed(false) {}
	virtual ~ResponseHttp() {}
	void Init();
	bool Redirection() { return (statusCode>=300 && statusCode<=307) ? true: false; }
	bool Success() { return statusCode==200 ? true : false; }
	const UrlHttp& Location() { return location; }
	char* GetMessage() { return &message[0]; }
	const size_t GetMsgSize() const { return messageSize; }
	const size_t GetMaxMsgSize() const { return message.size(); }
	char* GetMsgBody() { return &msgBody[0]; }
	const size_t GetMsgBodySize() const { return msgBody.size(); }
	const std::string GetHeaderValue(const std::string& label) const;
	void SetRecvAllDataOn() { recvAllMsgBody=true; }
	void SetRecvAllDataOff() { recvAllMsgBody=false; }
	void ParseRecvMessage(size_t aBytes);
	void Debug();
};

class Socket {
	int id;
public:
	Socket();
	~Socket();
	int Connect(const InetSockAddr& ip);
	int Send(const char* aData, const size_t aSize);
	int Receive(char* aData, const size_t aSize);
};

class SessionHttp {
	InetSockAddr ip;
	std::string base;
	Socket socket;
public:
	SessionHttp(){}
	SessionHttp(const UrlHttp& aAddr) : ip(aAddr.GetAddress()), base(aAddr.GetHost()) {}
	void Init(const UrlHttp& aAddr) { ip=aAddr.GetAddress(); base.assign(aAddr.GetHost()); }
	void SubmitAllRequest(const RequestHttp& request);
	void ReceiveAllResponse(ResponseHttp& response);
	bool SubmitRequest(const RequestHttp& request);
	bool ReceiveResponse(ResponseHttp& response);
};

class ResourceInfoSession {
	UrlHttp addr;
	size_t fileSize;
	bool acceptRanges;
public:
	ResourceInfoSession(const std::string& aAddr);
	const size_t GetFileSize() const { return fileSize;	}
	const UrlHttp& GetAddress() const { return addr; }
	const bool AcceptRanges() const { return acceptRanges; }
};

}

#endif /* NETWORK_H_ */
