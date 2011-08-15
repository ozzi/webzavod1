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

void Socket::Send(const Request& request)
{
}

void Socket::Receive(Response& responce)
{
}

InputInfo::InputInfo(const std::string& aUrl) : addr(aUrl)
{
	Http http(addr);
	http.SubmitRequest(HEADRequest(addr.GetResource()));
	HEADResponse head;
	http.ReceiveResponse(head);
	fileSize=head.GetContentLength();
}

bool Http::SubmitRequest(const Request& request)
{
	socket.Send(request);
	return false;
}

bool Http::ReceiveResponse(Response& responce)
{
	return false;
}

}
