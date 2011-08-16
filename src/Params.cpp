/*
 * Params.cpp
 *
 *  Created on: 10.08.2011
 *      Author: outz
 */

#include <stdlib.h>
#include "Params.h"
#include "Error.h"

namespace webzavod {

Params::Params(int argc, char *argv[])
{
	int longIndex;
	for (int opt(getopt_long(argc, argv, optString, longOpts, &longIndex)); opt!=-1; )
	{
		switch (opt)
		{
		case 't':
			threadsCount=atoi(optarg);
			if (!threadsCount)
				throw ThreadsErr();
			break;
		case 'b':
			bufferSize=atoi(optarg);
			if (!bufferSize)
				throw BufferErr();
			break;
		case 'o':
			output.assign(optarg);
			break;
		case 'h':
		default:
			throw UsageErr();
		}
		opt=getopt_long(argc, argv, optString, longOpts, &longIndex);
	}
	if (argc-optind!=1)
		throw UsageErr();
	url.assign(argv[optind]);
}

Params::~Params()
{
}

const char * Params::optString = "t:b:o:h";

const option Params::longOpts[] = {
	{ "threads-count", required_argument, NULL, 't' },
	{ "buffer-size", required_argument, NULL, 'b' },
	{ "output-file", required_argument, NULL, 'o' },
	{ "help", no_argument, NULL, 'h' },
	{ NULL, no_argument, NULL, 0 }
};

} /* namespace webzavod */
