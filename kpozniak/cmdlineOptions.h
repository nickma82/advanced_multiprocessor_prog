/*
 * cmdlineOptions.h
 *
 *  Created on: 11 May 2015
 *      Author: nickma
 */

#pragma once

#ifndef CMDLINEOPTIONS_H_
#define CMDLINEOPTIONS_H_

#include <map>
#include <string>
#include <getopt.h>
#include <sstream>
#include <ostream>

#define TESTVALUE 42
#define OPTARG_BOOL_ISAVAILABLE "1"
#define OPTARG_BOOL_ISMISSING	"0"

/**
 * that class is used as a type conversion proxy to
 * support return type std::string along with bool
 */
class ReturnProxy {
public:
	ReturnProxy(const std::string value) : value(value) {}

	//source of the stringToNumber function: http://www.cplusplus.com/forum/articles/9645/
	template <typename T>
	T stringToNumber ( const std::string &Text ) const//Text not by const reference so that the function can be used with a
	{//character array as argument
		std::istringstream ss(Text);
		T result;
		return ss >> result ? result : 0;
	}

	operator bool() const {	return (value == OPTARG_BOOL_ISAVAILABLE); }
	operator int() const { return stringToNumber<int>(value); }
	operator std::string() const { return value; }
private:
	std::string value;
};

inline std::ostream &operator<<(std::ostream &out, const ReturnProxy &rp) {
		out << (std::string)rp;
		return out;
}

class CommandLineOptions {
public:

	/** parses command line args originally given to main(...)
	 *  and manages them internally in a key-value map.
	 * After successful parsing the values are accessible via
	 *  getValue(..)
	 */
	int parse_args(int argc, char **argv);

	/** used to obtain a value parsed by CommandLineOptions::parse_args(..)
	 */
	ReturnProxy getValue(const std::string& key);

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
