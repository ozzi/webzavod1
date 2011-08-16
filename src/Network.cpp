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
#include <iostream>
#include "Error.h"

namespace webzavod {

void IP::SetAddr(const std::string& aAddr)
{
	//где-то здесь порылась собачка!!!!!!!!!!!!!!!!!!!!!!!!!!!
	hostent* he(gethostbyname(aAddr.c_str()));
	if (!he)
		throw GetHostByNameErr();
	in_addr inaddr;
	if (inet_aton(he->h_addr, &inaddr)==-1)
		throw InetAtonErr();
	addr=inaddr.s_addr;
}

Address::Address(const std::string& aUrl)
{
	//исходим из того, что в строке адреса всегда есть http
	std::string start("http://");
	size_t pos(aUrl.find(start));
	if (pos!=0)
		throw WrongleUrlErr();
	pos=aUrl.find('/',start.size());
	if (pos==std::string::npos)
		throw WrongleUrlErr();
	base.assign(aUrl, start.size(), pos-start.size());
	resource.assign(aUrl, pos, aUrl.length()-pos);
	ip.SetAddr(base);
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

int Socket::Connect(const IP& ip)
{
	sockaddr_in addr;
	addr.sin_addr=ip.Address();
	addr.sin_family=AF_INET;
	addr.sin_port=80;
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

void Response::CalculateHeader()
{
	if (recvSize)
	{
		std::string nn("\r\n\r\n");
		std::vector<char>::iterator posnn(std::search(buffer.begin(), buffer.begin()+recvSize, nn.begin(), nn.end()));
		header.insert(header.end(), buffer.begin(), posnn);
		if (posnn!=buffer.end())
		{
			headerReceived=true;
			data.assign(posnn+nn.size(),buffer.end());
		}
	}
	else
		headerReceived=true;
}

void Response::SetRecvSize(size_t bytes)
{
	recvSize=bytes;
	if (headerReceived)
		data.assign(buffer.begin(), buffer.begin()+recvSize);
	else
		CalculateHeader();
}

const std::string Response::GetLabelValue(const std::string& label) const
{
	std::string value;
	std::vector<char>::const_iterator posLabel(std::search(header.begin(), header.end(), label.begin(), label.end()));
	if (posLabel!=header.end())
	{
		std::string n("\r\n");
		std::vector<char>::const_iterator posn(std::search(posLabel+label.size(), header.end(), n.begin(), n.end()));
		value.assign(posLabel+label.size(), posn);
	}
	return value;
}

InputInfo::InputInfo(const std::string& aUrl) : addr(aUrl)
{
	Http http(addr);
	http.SubmitAllRequest(HEADRequest(addr.GetResource()));
	Response head;
	http.ReceiveResponse(head);
	fileSize=atoi(head.GetLabelValue("Content-Length:").c_str());
	//!!!!!!!!!!!!!!!!! очень важно обработать здеся ответы сервера все возможные
	//!!!!!!!!!!!!!!!!! очень важно обработать здеся ответы сервера все возможные
	//!!!!!!!!!!!!!!!!! очень важно обработать здеся ответы сервера все возможные
	//!!!!!!!!!!!!!!!!! очень важно обработать здеся ответы сервера все возможные
	//!!!!!!!!!!!!!!!!! очень важно обработать здеся ответы сервера все возможные
	//!!!!!!!!!!!!!!!!! очень важно обработать здеся ответы сервера все возможные
	//!!!!!!!!!!!!!!!!! очень важно обработать здеся ответы сервера все возможные
	//!!!!!!!!!!!!!!!!! очень важно обработать здеся ответы сервера все возможные
	//location сюды инсертиться
}

void Http::SubmitAllRequest(const Request& request)
{
	unsigned total(0);
	int bytes(0);
	socket.Connect(ip);
	while (total<request.Get().size())
	{
		bytes=socket.Send(request.Get().c_str()+total, request.Get().size()-total);
		if (bytes==-1)
			throw SendSocketErr();
		total+=bytes;
	}
}

bool Http::ReceiveResponse(Response& response)
{
	int bytes(socket.Receive(response.GetBuffer(), response.GetBufferSize()));
	if (bytes==-1)
		throw RecvSocketErr();
	response.SetRecvSize(bytes);
	return bytes==0;
}

}
