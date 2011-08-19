//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================

#include <cstdlib>
#include "Params.h"
#include "Error.h"

namespace webzavod {

Params::Params(int argc, char *argv[])
	: threadsCount(0), refresh(2)
{
	int longIndex;
	for (int opt(getopt_long(argc, argv, optString, longOpts, &longIndex)); opt!=-1; )
	{
		switch (opt)
		{
		case 't':
			threadsCount=atoi(optarg);
			if (!threadsCount)
				throw ParseThreadsNumberErr();
			break;
		case 'r':
			refresh=atoi(optarg);
			if (!refresh)
				throw ParseUpdateIntervalErr();
			break;
		case 'o':
			output.assign(optarg);
			break;
		case 'h':
			throw ParseHelpErr();
		default:
			throw ParseUsageErr();
		}
		opt=getopt_long(argc, argv, optString, longOpts, &longIndex);
	}
	if (argc-optind!=1)
		throw ParseUsageErr();
	url.assign(argv[optind]);
	if (!threadsCount)
		throw ParseThreadsNumberErr();
}

Params::~Params()
{
}

const char * Params::optString = "t:o:r:h";

const option Params::longOpts[] = {
	{ "threads-count", required_argument, NULL, 't' },
	{ "output-file", required_argument, NULL, 'o' },
	{ "refresh-time", required_argument, NULL, 'r' },
	{ "help", no_argument, NULL, 'h' },
	{ NULL, no_argument, NULL, 0 }
};

} /* namespace webzavod */
