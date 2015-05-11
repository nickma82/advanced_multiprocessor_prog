/*
 * cmdlineOptions.cc
 *
 *  Created on: 11 May 2015
 *      Author: nickma
 */


//#include <unistd.h>

#include <sys/ioctl.h>
#include <stdint.h>
#include <stdio.h>
#include <iostream>
#include "cmdlineOptions.h"


using namespace std;

const std::string CommandLineOptions::getValue(const std::string& key) const {
	auto it =_optionsMap.find(key);

	/* if found return */
	if (it != _optionsMap.end())
		return it->second;
	return "";
}

void CommandLineOptions::usage() {
	printf("Usage: \n");
	printf("valid arguments are:\n");
	printf("\t --add --remove --random\n");

	std::cout << "\t --implementation={FGL, OS, LS, LBS, REF}" << std::endl;
	std::cout << "\t --threadCount=[1,512]" << std::endl;
	std::cout << "\t --repeatCycles=[0,maxof(int)]" << std::endl;
}


int CommandLineOptions::parse_args(int argc, char **argv) {
	if(argc <= 1) {
		usage();
		return -1;
	}

//	size_t base_options = sizeof(long_options_common) / sizeof(option);

	while (1) {
		const char shortOps[] = "0:";
		tmpValue = -1;

		/* getopt_long stores the option index here. */
		int option_index = -1;
		int optChar = getopt_long_only(argc, argv, shortOps, long_options_common, &option_index);
		/* Detect the end of the options. */
		if (optChar == -1)
			break;

		switch (optChar) {
		case 0: {
			std::string optionName(long_options_common[option_index].name);
			std::cout << "option '" <<  optionName << "'";

			if (optarg) {
				printf(" with arg %s", optarg);
				auto retPair = _optionsMap.insert ( std::pair<std::string,std::string>(optionName,optarg) );
				if (retPair.second == false)
					std::cout << " element '" << optionName << "' already existed";
			}

			if ( (option_index >= 0) && (optarg) && (tmpValue >= 0) ) {
				std::cout << " additionally tmpValue is set to: " << tmpValue;
			}

			std::cout << std::endl;
			break;
		}
		case '?':
			/* getopt_long already printed an error message. */
			usage();
			return -1;
		default:
			fprintf(stderr, "Unrecognized option: '-%c'\n", optChar);
			usage();
			return -1;
		}
	}

	/* Print any remaining command line arguments (not options). */
	if (optind < argc) {
		printf("non-option ARGV-elements: ");
		while (optind < argc)
			printf("%s ", argv[optind++]);
		printf("\n");
	}

	return 0;
}


