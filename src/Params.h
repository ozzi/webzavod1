/*
 * Params.h
 *
 *  Created on: 10.08.2011
 *      Author: outz
 */

#ifndef PARAMS_H_
#define PARAMS_H_
#include <string>
#include <getopt.h>

namespace webzavod {

class Params {
	static const char *optString;
	static const option longOpts[];

	std::string url;
	std::string output;
	unsigned threadsCount;
	unsigned bufferSize;

public:
	Params(int argc, char *argv[]);
	const unsigned GetThreadsCount() const { return threadsCount; }
	const unsigned GetBufferSize() const { return bufferSize; }
	const std::string& GetOutput() const { return output; }
	const std::string& GetUrl() const { return url; }
	virtual ~Params();
};

} /* namespace webzavod */
#endif /* PARAMS_H_ */
