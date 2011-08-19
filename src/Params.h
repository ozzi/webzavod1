//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================

#ifndef PARAMS_H_
#define PARAMS_H_
#include <string>
#include <getopt.h>

namespace webzavod {

class Params {
	//класс, обрабатывающий командную строку и извлекающий из нее значения входных параметров
	static const char *optString;
	static const option longOpts[];

	std::string url;
	std::string output;
	unsigned threadsCount;
	unsigned refresh;

public:
	Params(int argc, char *argv[]);
	const unsigned GetThreadsCount() const { return threadsCount; }
	const unsigned GetRefresh() const { return refresh; }
	const std::string& GetOutput() const { return output; }
	const std::string& GetUrl() const { return url; }
	virtual ~Params();
};

} /* namespace webzavod */
#endif /* PARAMS_H_ */
