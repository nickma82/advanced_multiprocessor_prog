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
	CommandLineOptions cmdOption = CommandLineOptions();
	int rc = cmdOption.parse_args(argc, argv);
	if (rc < 0)
		return -1;

	std::cout << cmdOption.getValue("implementation") << std::endl; // prints !!!Hello World!!!
	return 0;
}
