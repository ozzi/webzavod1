/*
 * Network.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#include <string>

namespace webzavod {

class Network {
public:
	Network(const std::string & aUrl);
	virtual ~Network();
};

}

#endif /* NETWORK_H_ */
