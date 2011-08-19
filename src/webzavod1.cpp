//============================================================================
// Author      : Alexander Zhukov
// Version     : 0.0a
// Copyright   : MIT license
//============================================================================

#include "Params.h"
#include "Program.h"
#include "Error.h"
#include <iostream>

int main(int argc, char *argv[]) {
	try
	{
		webzavod::Program prog(webzavod::Params(argc, argv));
		prog.Work();
	}
	catch (webzavod::Error &err)
	{
		std::cout<<err.Print();
	}
	return 0;
}
