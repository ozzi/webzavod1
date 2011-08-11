/*
 * Params.cpp
 *
 *  Created on: 10.08.2011
 *      Author: outz
 */

#include "Params.h"

namespace webzavod {

const bool Params::Init(int argc, char *argv[])
{
	return true;
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
