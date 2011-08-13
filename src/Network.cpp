/*
 * Network.cpp
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#include "Network.h"
#include <netdb.h>
#include "Error.h"

namespace webzavod {

Address::Address(const std::string& aUrl)
{
	size_t pos(aUrl.find('\''));
	if (pos==std::string::npos)
		throw WrongleUrlErr();
	base.assign(aUrl, 0, pos);
	resource.assign(aUrl, pos, aUrl.length()-pos);
}

HeadRequest::HeadRequest(const Address& addr)
{
}

void Socket::Send(const Request& request)
{
}

void Socket::Receive(Response& responce)
{
}

const Response Http::Head()
{
	socket.Send(HeadRequest(addr));
	Response meta;
	socket.Receive(meta);
	return meta;
}

InputInfo::InputInfo(const std::string& aUrl) : addr(aUrl)
{
	Http http(addr);
	Response metadata(http.Head());
	fileSize=metadata.GetContentLength();
}

}
