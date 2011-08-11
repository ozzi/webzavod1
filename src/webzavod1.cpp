//============================================================================
// Name        : webzavod1.cpp
// Author      : Alexander Zhukov
// Version     :
// Copyright   : MIT license
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Params.h"

int main(int argc, char *argv[]) {
	webzavod::Params params;
	bool res(params.Init(argc, argv));
	return res;
}
