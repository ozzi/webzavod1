/*
 * Filesystem.h
 *
 *  Created on: 11.08.2011
 *      Author: outz
 */

#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <string>

namespace webzavod {

class Filesystem {
public:
	Filesystem(const std::string & aName);
	virtual ~Filesystem();
};

}

#endif /* FILESYSTEM_H_ */
