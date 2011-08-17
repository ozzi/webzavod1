/*
 * Network.cpp
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#include "Network.h"
#include <algorithm>
#include <netdb.h>
#include <arpa/inet.h>
#include "Error.h"
#include <iostream>

namespace webzavod {

InetSockAddr::InetSockAddr(const std::string& aAddr, const std::string& aPort)
{
	SetIpv4Addr(aAddr, aPort);
}

const sockaddr_in& InetSockAddr::GetIpv4Addr() const
{
	return address;
}

void InetSockAddr::SetIpv4Addr(const std::string& aAddr, const std::string& aPort)
{
	addrinfo hint, *info;
	std::fill((char*)&hint, (char*)&hint+sizeof(hint), 0);
	hint.ai_family=AF_INET;
	hint.ai_socktype=SOCK_STREAM;
	hint.ai_protocol=IPPROTO_TCP;
	int err(getaddrinfo(aAddr.c_str(), aPort.c_str(), &hint, &info));
	if (err)
		throw GetAddrInfoErr(err);
	std::copy((char*)info->ai_addr, (char*)info->ai_addr+info->ai_addrlen, (char*)&address);
}

UrlHttp::UrlHttp(const std::string& aUrl)
{
	//исходим из того, что в строке адреса всегда есть http
	std::string start("http://");
	size_t pos(aUrl.find(start));
	if (pos!=0)
		throw WrongleUrlErr();
	pos=aUrl.find('/',start.size());
	if (pos==std::string::npos)
		throw WrongleUrlErr();
	host.assign(aUrl, start.size(), pos-start.size());
	uri.assign(aUrl, pos, aUrl.length()-pos);
	address.SetIpv4Addr(host, "80");
}

Socket::Socket()
{
	id=socket(AF_INET, SOCK_STREAM, 0);
	if (id==-1)
		throw CreateSocketErr();
}

Socket::~Socket()
{
	close(id);
}

int Socket::Connect(const InetSockAddr& ip)
{
	sockaddr_in addr(ip.GetIpv4Addr());
	return connect(id, (sockaddr*)&addr, sizeof(addr));
}

int Socket::Send(const char* aData, const size_t aSize)
{
	return send(id, aData, aSize, 0);
}

int Socket::Receive(char* aData, const size_t aSize)
{
	return recv(id, aData, aSize, 0);
}

void ResponseHttp::Init()
{
	startLine.clear();
	headersLine.clear();
	headers.clear();
	msgBody.clear();
	messageSize=0;
	startLineProcessed=false;
	headersLineProcessed=false;
	statusCode=0;
}

void ResponseHttp::ParseRecvMessage(size_t aBytes)
{
	messageSize=aBytes;
	beginParse=message.begin();
	if (!startLineProcessed)
		GetStartLine();
	if (!headersLineProcessed)
		GetHeadersLine();
	GetMessageBody();
}


void ResponseHttp::GetStartLine()
{
	std::string rn("\r\n");
	std::vector<char>::iterator start_line_end(std::search(message.begin(), message.begin()+messageSize, rn.begin(), rn.end()));
	startLine.assign(message.begin(),start_line_end);
	beginParse=start_line_end+rn.size();
	std::string http_str("HTTP/");
	if (startLine.find(http_str))
		return;//ошибочка вышла
	version.assign(startLine, http_str.size(), 3);
	std::string status_code_str(startLine, http_str.size()+4, 3);
	statusCode=atoi(status_code_str.c_str());
	startLine.clear();
	startLineProcessed=true;
}

void ResponseHttp::GetHeadersLine()
{
	std::string rnrn("\r\n\r\n");
	std::vector<char>::iterator message_end(message.begin()+messageSize);
	std::vector<char>::iterator headers_line_end(std::search(beginParse, message_end, rnrn.begin(), rnrn.end()));
	headersLine.insert(headersLine.end(), beginParse, headers_line_end);
	if (headers_line_end!=message_end)
	{
		headersLineProcessed=true;
		beginParse=headers_line_end+rnrn.size();
		ParseHeaders();
	}
}

void ResponseHttp::ParseHeaders()
{
	std::string rn("\r\n");
	std::string separator(": ");
	std::string name;
	std::string value;
	size_t begin_name(0), begin_value(0);
	do
	{
		begin_value=headersLine.find(separator, begin_name);
		if (begin_value==std::string::npos)
			break;//invalid header
		name.assign(headersLine, begin_name, begin_value-begin_name);
		begin_value+=separator.size();
		begin_name=headersLine.find(rn, begin_value);
		value.assign(headersLine, begin_value, begin_name-begin_value);
		if (begin_name!=std::string::npos)
			begin_name+=rn.size();
		headers[name]=value;
	} while (begin_name!=std::string::npos);
	headersLineProcessed=true;
	headersLine.clear();
}

void ResponseHttp::GetMessageBody()
{
	if (recvAllMsgBody)
		msgBody.insert(msgBody.end(), beginParse, message.begin()+messageSize);
	else
		msgBody.assign(beginParse, message.begin()+messageSize);
}

const std::string ResponseHttp::GetHeaderValue(const std::string& aName) const
{
	std::string res;
	std::map<std::string, std::string>::const_iterator it(headers.find(aName));
	if (it!=headers.end())
		res=it->second;
	return res;
}

void ResponseHttp::Debug()
{
	std::stringstream ss;
	ss<<"HTTP/"<<version<<" "<<statusCode<<std::endl;
	std::map<std::string,std::string>::iterator it(headers.begin());
	while (it!=headers.end())
	{
		ss<<it->first<<": "<<it->second<<std::endl;
		it++;
	}
	std::cout<<ss.str();
}

void SessionHttp::SubmitAllRequest(const RequestHttp& request)
{
	unsigned total(0);
	int bytes(0);
	socket.Connect(ip);
	int n=request.Get().size();
	while (total<n)
	{
		bytes=socket.Send(request.Get().c_str()+total, n-total);
		if (bytes==-1)
			throw SendSocketErr();
		total+=bytes;
	}
}

void SessionHttp::ReceiveAllResponse(ResponseHttp& response)
{
	response.SetRecvAllDataOn();
	int bytes;
	do {
		bytes=socket.Receive(response.GetMessage(), response.GetMaxMsgSize());
		if (bytes==-1)
			throw RecvSocketErr();
		response.ParseRecvMessage(bytes);
	} while (bytes);
}

bool SessionHttp::ReceiveResponse(ResponseHttp& response)
{
	response.SetRecvAllDataOff();
	int bytes(socket.Receive(response.GetMessage(), response.GetMaxMsgSize()));
	if (bytes==-1)
		throw RecvSocketErr();
	response.ParseRecvMessage(bytes);
	return bytes==0;
}

ResourceInfoSession::ResourceInfoSession(const std::string& aUrl) : addr(aUrl)
{
	SessionHttp http;
	HEADRequestHttp head_request;
	ResponseHttp head_response;
	do
	{
		http.Init(addr);
		head_request.Init(addr);
		http.SubmitAllRequest(head_request);
		head_response.Init();
		http.ReceiveAllResponse(head_response);
		head_response.Debug();
		if (head_response.Redirection())
			addr=head_response.Location();
	} while (head_response.Redirection());
	if (!head_response.Success())
		throw ResourceInfoErr();
	std::string str_size(head_response.GetHeaderValue("Content-Length"));
	fileSize=atoi(str_size.c_str());
}

}
