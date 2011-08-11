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

int main(int argc, char *argv[]) {
	webzavod::Params params;
	params.Init(argc, argv);
	webzavod::Program prog;
	prog.Init(params);
	prog.Work();
	prog.Close();
	return true;
}
