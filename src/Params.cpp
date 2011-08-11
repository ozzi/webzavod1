/*
 * Params.cpp
 *
 *  Created on: 10.08.2011
 *      Author: outz
 */

#include "Params.h"
#include <stdlib.h>

namespace webzavod {


//надо оформить код с использованием исключений и с выводом хелпа в поток
const bool Params::Init(int argc, char *argv[])
{
	bool res(false);
	int longIndex;
	for (int opt(getopt_long(argc, argv, optString, longOpts, &longIndex)); opt!=-1; opt=getopt_long(argc, argv, optString, longOpts, &longIndex))
	{
		switch (opt)
		{
		case 't':
			threadsCount=atoi(optarg);
			break;
		case 'b':
			bufferSize=atoi(optarg);
			break;
		case 'o':
			outputFileName.assign(optarg);
			break;
		case 'h':
			//<<usage();
			break;
		default:
			break;
		}
	}
	if (argc-optind==1)
	{
		url.assign(*(argv+optind));
		res=true;
	}
	return res;
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
