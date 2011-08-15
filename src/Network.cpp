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

namespace webzavod {

void IP::SetAddr(const std::string& aAddr)
{
	hostent* he(gethostbyname(aAddr.c_str()));
	if (!he)
		throw GetHostByNameErr();
	in_addr inaddr;
	if (inet_aton(he->h_addr_list[0], &inaddr)==-1)
		throw InetAtonErr();
	addr=inaddr.s_addr;
}

Address::Address(const std::string& aUrl)
{
	size_t pos(aUrl.find('\''));
	if (pos==std::string::npos)
		throw WrongleUrlErr();
	base.assign(aUrl, 0, pos);
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

Request::Request()
{

}

HEADRequest::HEADRequest(const std::string& aResource)
{

}

GETRequest::GETRequest(const std::string& aResource)
{

}

PartialGETRequest::PartialGETRequest(const std::string& aResource, const size_t aRange , const size_t aBytes) : GETRequest(aResource)
{

}

void Response::CalculateHeader()
{
	if (recvSize)
	{
		std::string nn("\n\n");
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
		std::string n("\n");
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
}

void Http::SubmitAllRequest(const Request& request)
{
	unsigned total(0);
	int bytes(0);
	while (total<request.Size())
	{
		bytes=socket.Send(request.GetData()+total, request.Size()-total);
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
