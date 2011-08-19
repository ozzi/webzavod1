//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================

#include "Network.h"
#include "Error.h"
#include <algorithm>
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>

namespace webzavod {

InetSockAddr::InetSockAddr(const std::string& aAddr, const std::string& aPort)
{
	SetIpv4Addr(aAddr, aPort);
}

const sockaddr_in& InetSockAddr::GetIpv4Addr() const
{
	return address;
}

const std::string InetSockAddr::GetIpv4AddrStr() const
{
	char buf[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &address, buf, INET_ADDRSTRLEN);
	return buf;
}

void InetSockAddr::SetIpv4Addr(const std::string& aAddr, const std::string& aPort)
{
	addrinfo hint, *info;
	std::fill((char*)&hint, (char*)&hint+sizeof(hint), 0);
	hint.ai_family=AF_INET;
	hint.ai_socktype=SOCK_STREAM;
	hint.ai_protocol=IPPROTO_TCP;
	hint.ai_flags=AI_PASSIVE;
	int err(getaddrinfo(aAddr.c_str(), aPort.c_str(), &hint, &info));
	if (err)
		throw GetAddrInfoErr(err);
	std::copy((char*)info->ai_addr, (char*)info->ai_addr+info->ai_addrlen, (char*)&address);
	freeaddrinfo(info);
}

UrlHttp::UrlHttp(const std::string& aUrl)
{
	Set(aUrl);
}

void UrlHttp::Set(const std::string& aUrl)
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
	id=0;
}

Socket::~Socket()
{
	Close();
}

void Socket::Connect(const InetSockAddr& ip)
{
	id=socket(AF_INET, SOCK_STREAM, 0);
	if (id==-1)
		throw CreateSocketErr();
	sockaddr_in addr(ip.GetIpv4Addr());
	if (connect(id, (sockaddr*)&addr, sizeof(addr))!=0)
		throw SocketConnectErr();
}

int Socket::Send(const char* aData, const size_t aSize)
{
	return send(id, aData, aSize, 0);
}

int Socket::Receive(char* aData, const size_t aSize)
{
	return recv(id, aData, aSize, 0);
}

void Socket::Close()
{
	close(id);
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
		ParseStartLine();
	if (!headersLineProcessed)
		ParseHeadersLine();
	ParseMessageBody();
}


void ResponseHttp::ParseStartLine()
{
	std::string rn("\r\n");
	std::vector<char>::iterator start_line_end(std::search(message.begin(), message.begin()+messageSize, rn.begin(), rn.end()));
	startLine.assign(message.begin(),start_line_end);
	beginParse=start_line_end+rn.size();
	std::string http_str("HTTP/");
	if (startLine.find(http_str))
		throw UnsuccessfulServerResponseErr(startLine);
	version.assign(startLine, http_str.size(), 3);
	startLine.erase(0, http_str.size()+version.size()+1);
	statusCode=atoi(std::string(startLine, 0, 3).c_str());
	startLineProcessed=true;
}

void ResponseHttp::ParseHeadersLine()
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

char ToLower(char c) { return std::tolower(c); }

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
			throw Error();//invalid header
		name.assign(headersLine, begin_name, begin_value-begin_name);
		std::transform(name.begin(), name.end(), name.begin(), ToLower);
		begin_value+=separator.size();
		begin_name=headersLine.find(rn, begin_value);
		value.assign(headersLine, begin_value, begin_name-begin_value);
		if (begin_name!=std::string::npos)
			begin_name+=rn.size();
		headers[name]=value;
	} while (begin_name!=std::string::npos);
	headersLineProcessed=true;
	headersLine.clear();
	if (Redirection())
	{
		std::string loc_str("location");
		std::map<std::string, std::string>::iterator it(headers.find(loc_str));
		if (it==headers.end())
			throw UnsuccessfulServerResponseErr(GetStartLine()+"\n"+GetStringHeaders());
		location.Set(it->second);
		std::cout<<loc_str<<": "<<it->second<<"\n";//debug
	}
}

void ResponseHttp::ParseMessageBody()
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

const std::string ResponseHttp::GetStringHeaders()
{
	std::stringstream ss;
	ss<<"HTTP/"<<version<<" "<<statusCode<<std::endl;
	std::map<std::string,std::string>::iterator it(headers.begin());
	while (it!=headers.end())
	{
		ss<<it->first<<": "<<it->second<<std::endl;
		it++;
	}
	return ss.str();
}

void SessionHttp::SubmitAllRequest(const RequestHttp& request)
{
	socket.Connect(ip);
	int n(request.Get().size());
	int total(0), bytes(0);
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

void SessionHttp::ConnectionClose()
{
	socket.Close();
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
		http.ConnectionClose();
		if (head_response.Redirection())
			addr=head_response.Location();
	} while (head_response.Redirection());
	if (!head_response.Success())
		throw UnsuccessfulServerResponseErr(head_response.GetStartLine());
	std::string hint(head_response.GetHeaderValue("content-length"));
	contentLength=atol(hint.c_str());
	std::cout<<"content-length: "<<contentLength<<" bytes ["<<head_response.GetHeaderValue("content-type")<<"]\n";
	fileName.assign(addr.GetUri().begin()+addr.GetUri().rfind("/")+1, addr.GetUri().end());
	std::string accept_ranges(head_response.GetHeaderValue("accept-ranges"));
	if ((!accept_ranges.size()) || (!accept_ranges.compare("none")))
	{
		std::cout<<"accept-ranges: none; multithreading off\n";
		acceptRanges=false;
	}
	else
	{
		std::cout<<"accept-ranges: "<<accept_ranges<<"; multithreading on\n";
		acceptRanges=true;
	}
}

}
