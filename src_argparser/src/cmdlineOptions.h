/*
 * cmdlineOptions.h
 *
 *  Created on: 11 May 2015
 *      Author: nickma
 */

#ifndef CMDLINEOPTIONS_H_
#define CMDLINEOPTIONS_H_

#include <map>
#include <string>
#include <getopt.h>

#define TESTVALUE 42

class CommandLineOptions {
public:

	/* parses command line args originally given to main(...)
	 *  and manages them internally in a key-value map.
	 * After successful parsing the values are accessible via
	 *  getValue(..)
	 */
	int parse_args(int argc, char **argv);

	const std::string getValue(const std::string& key) const;

private:
	//see c++11 uniform initialization, note size must be given
	const struct option long_options_common[7] {
		//{"foo", 		no_argument, &tmpInteger, 1}, //flag
		{"add", 	no_argument, nullptr, 0},
		{"remove", 	no_argument, nullptr, 0},
		{"random", 	no_argument, nullptr, 0},

		//{"implementation",	required_argument, &tmpValue, TESTVALUE},
		{"implementation",	required_argument, nullptr, 0},
		{"threadCount", 	required_argument, nullptr, 0},
		{"repeatCycles",	required_argument, nullptr, 0},
		{0, 0, 0, 0}
	};

	int tmpValue = -1;
	std::map<std::string, std::string> _optionsMap;

	void usage();
};

#endif /* CMDLINEOPTIONS_H_ */
