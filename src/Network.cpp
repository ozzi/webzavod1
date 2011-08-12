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

Network::Network(const Source& source)
{//устанавливаем соединение с требуемым ресурсом, с помощью HEAD узнаем размер файла
	throw GetHostByNameErr();
}

void Network::Get(Buffer & buffer)
{

}

const bool Network::Connected() const
{
	return false;
}

void Network::Init()
{
}

Network::~Network() {
}

}
