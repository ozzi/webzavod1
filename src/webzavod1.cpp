//============================================================================
// Name        : webzavod1.cpp
// Author      : Alexander Zhukov
// Version     :
// Copyright   : MIT license
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Params.h"
#include "Program.h"
#include "Error.h"

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
