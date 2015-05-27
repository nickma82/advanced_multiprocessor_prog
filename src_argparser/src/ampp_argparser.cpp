//============================================================================
// Name        : ampp_argparser.cpp
// Author      : Nick Ma
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>

#include "cmdlineOptions.h"

int main (int argc, char **argv) {
	CommandLineOptions cmdOption;
	int rc = cmdOption.parse_args(argc, argv);
	if (rc < 0)
		return -1;

	std::cout << "implementation is: " << cmdOption.getValue("implementation") << std::endl;
	if(cmdOption.getValue("add")) {
		bool add = cmdOption.getValue("add");
		std::cout << "add is: " << add << std::endl;
	}

	std::cout << "threadCount:" << (int)cmdOption.getValue("threadCount") << std::endl;
	return 0;
}
