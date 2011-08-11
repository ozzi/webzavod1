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

	unsigned threadsCount;
	unsigned bufferSize;
	std::string outputFileName;
	std::string url;
public:
	Params(int argc, char *argv[]);
	const unsigned GetThreadsCount() const { return threadsCount; }
	const unsigned GetBufferSize() const { return bufferSize; }
	const std::string & GetOutputFileName() const { return outputFileName; }
	const std::string & GetUrl() const { return url; }
	virtual ~Params() {};
};

} /* namespace webzavod */
#endif /* PARAMS_H_ */
