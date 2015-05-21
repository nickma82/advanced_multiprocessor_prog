#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include <inttypes.h> //for strtoumax
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <unistd.h>
#include <iomanip>

#include <string.h>
#include <vector>
#include <map>
#include <memory>

#include <atomic>
#include <thread>
#include <future>

#include <sys/time.h>
//#include <time.h>
#include <chrono>

#define BILLION 1000000000
int timeDiff(struct timespec *result, struct timespec *x,
		struct timespec *y) {
	/* Perform the carry for the later subtraction by updating y. */
	if (x->tv_nsec < y->tv_nsec) {
		__syscall_slong_t nsec = (y->tv_nsec - x->tv_nsec) / BILLION + 1;
		y->tv_nsec -= BILLION * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_nsec - y->tv_nsec > BILLION) {
		__syscall_slong_t nsec = (x->tv_nsec - y->tv_nsec) / BILLION;
		y->tv_nsec += BILLION * nsec;
		y->tv_sec -= nsec;
	}

	/* Compute the time remaining to wait.
	 tv_nsec is certainly positive. */
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_nsec = x->tv_nsec - y->tv_nsec;

	/* Return 1 if result is negative. */
	return x->tv_sec < y->tv_sec;
}

int main(int argc, char **argv) {
	struct timespec result, tmpTimeNow, mytime1, highPrecision1, timeOfDay;

	//
	clock_gettime(CLOCK_MONOTONIC, &mytime1);
	for(int i = 0; i<999; ++i) {
		std::chrono::steady_clock::now();
	}
	clock_gettime(CLOCK_MONOTONIC, &tmpTimeNow);
	timeDiff(&result, &tmpTimeNow, &mytime1);
	std::cout << "1000 times std::chrono::steady_clock::now(): \t\t" << result.tv_sec << "." << std::setfill ('0') << std::setw (9) << result.tv_nsec << std::endl;


	clock_gettime(CLOCK_MONOTONIC, &highPrecision1);
	for(int i = 0; i<999; ++i) {
		std::chrono::high_resolution_clock::now();
	}
	clock_gettime(CLOCK_MONOTONIC, &tmpTimeNow);
	timeDiff(&result, &tmpTimeNow, &highPrecision1);
	std::cout << "1000 times std::chrono::high_resolution_clock::now(): \t" << result.tv_sec << "." << std::setfill ('0') << std::setw (9) << result.tv_nsec << std::endl;


	struct timeval start;
	clock_gettime(CLOCK_MONOTONIC, &timeOfDay);
	for(int i = 0; i<999; ++i) {
		gettimeofday(&start, NULL);
	}
	clock_gettime(CLOCK_MONOTONIC, &tmpTimeNow);
	timeDiff(&result, &tmpTimeNow, &timeOfDay);
	std::cout << "1000 times gettimeofday: \t\t\t\t" << result.tv_sec << "." << std::setfill ('0') << std::setw (9) << result.tv_nsec << std::endl;

	return 0;
}
