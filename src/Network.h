//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================
//набор классов реализующих сетевую подсистему

#ifndef NETWORK_H_
#define NETWORK_H_

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

namespace webzavod {

class InetSockAddr {
	//преобразуем сетевое имя в sockaddr_in
	//реализовано только для ipv4
	sockaddr_in address;
public:
	InetSockAddr(){}
	InetSockAddr(const std::string& aAddr, const std::string& aPort);
	~InetSockAddr(){}
	void SetIpv4Addr(const std::string& aAddr, const std::string& aPort);
	const sockaddr_in& GetIpv4Addr() const;
	const std::string GetIpv4AddrStr() const;
};

class UrlHttp {
	//класс парсит строку адреса на адрес сокета, имя сайта и путь к ресурсу
	InetSockAddr address;
	std::string host;
	std::string uri;
public:
	UrlHttp(){}
	UrlHttp(const std::string& aUrl);
	~UrlHttp() {}
	void Set(const std::string& aUrl);
	const std::string& GetHost() const { return host; }
	const std::string& GetUri() const { return uri; }
	const InetSockAddr& GetAddress() const { return address; }
};

class RequestHttp {
	//с помощью этого класса делается строка-запрос к серверу
	std::string version;
	std::string uri;
	std::string host;
protected:
	const std::string _get(const char* method) const
	{
		std::stringstream get;
		get << method << " " << uri << " HTTP/" << version << "\n";
		get << "Host: " << host << "\n";
		get << "Accept: */*\n";
		get << "Referer: http://" << host << "/\n";
		return get.str();
	}
public:
	RequestHttp() : version("1.0") {}
	RequestHttp(const UrlHttp& aUrl) : version("1.0"), uri(aUrl.GetUri()), host(aUrl.GetHost()) {}
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
		: GETRequestHttp(aUrl), beginRange(aRange), endRange(aRange + aBytes -1) {}
	virtual ~PartialGETRequestHttp() {}
	virtual const std::string Get() const
	{
		std::stringstream get;
		get << _get("GET") << "Range: bytes=" << beginRange << "-" << endRange << "\n\n";
		return get.str();
	}
};

class ResponseHttp {
	//класс парсит сообщение полученное от сервера
	std::vector<char> message;//raw data
	std::string startLine;//стартовая строка
	std::string headersLine;//строка заголовков
	std::map<std::string, std::string> headers;//заголовки в удобной форме
	std::vector<char> msgBody;//сами запрошенные данные
	size_t messageSize;//размер полученного сообщения message
	std::string version;//версия протокола http сервера
	bool startLineProcessed;//стартовая строка обработана
	bool headersLineProcessed;//заголовки обработаны
	bool recvAllMsgBody;//флаг, определяющий, принимать ли файл сразу, либо по кускам
	int statusCode;
	UrlHttp location;//при перенаправлении здесь новый адрес ресурса
	std::vector<char>::iterator beginParse;//указатель на нераспарсенные данные
	void ParseStartLine();//парсим стартовую строку
	void ParseHeadersLine();//парсим строку с заголовками
	void ParseHeaders();//создаем карту заголовков
	void ParseMessageBody();//парсим тело сообщения (файл)

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
	const std::string GetStringHeaders();
	const std::string& GetStartLine() {return startLine; }
};

class Socket {
	//небольшая обертка над системными вызовами socket
	int id;
public:
	Socket();
	~Socket();
	void Connect(const InetSockAddr& ip);
	int Send(const char* aData, const size_t aSize);
	int Receive(char* aData, const size_t aSize);
	void Close();
};

class SessionHttp {
	//класс реализующий общение с сервером по протоколу http
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
	void ConnectionClose();
};

class ResourceInfoSession {
	//класс устанавливающий первоначальное соединение с сервером
	//и с помощью запросов HEAD устанавливающий сведения о запрашиваемом ресурсе
	UrlHttp addr;
	size_t contentLength;
	bool acceptRanges;
	std::string fileName;
public:
	ResourceInfoSession(const std::string& aAddr);
	const size_t GetFileSize() const { return contentLength; }//возвращает размер файла, хранящегося на сервере
	const std::string& GetFileName() const { return fileName; }
	const UrlHttp& GetAddress() const { return addr; }
	const bool AcceptRanges() const { return acceptRanges; }//возможна ли многопотоковая закачка
};

}

#endif /* NETWORK_H_ */
