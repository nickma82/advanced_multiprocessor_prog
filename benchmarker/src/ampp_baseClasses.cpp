//============================================================================
// Name        : ampp_baseClasses.cpp
// Author      : Nick Ma
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stddef.h>
#include <thread>

#include "ValueAnalyser.h"
#include "TimeMeasurement.h"
#include "TimeValues.h"
#include "ValueStore.h"

int main() {
//	std::cout << "### Bare Timer Value tests" << std::endl;
//	TimerValue tv(std::chrono::seconds(1), ID_INSERT);
//	tv.printValues();
//	ThreadTimerValue ttv(tv, 5);
//	ttv.printValues();
//	ThreadTimerValue ttv2(std::chrono::seconds(1), ID_INSERT, 3);
//	ttv2.printValues();


	std::cout << "### TimeMeasure tests" << std::endl;
	StartStopTimer measure(ID_INSERT);
	measure.time_start();
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	measure.time_stop();
	std::cout << "Time needed:" << measure.getTimeSpan().count() << std::endl;


	std::cout << "### TimerStore tests, which EVERY THREAD uses to store different time measurements" << std::endl;
	const unsigned int thread1_id = 42;
	ValueStore tsThread1;
	tsThread1.addTimeMeasurement(measure);
	tsThread1.addTimeMeasurement(measure);
	tsThread1.addMeasurementWithoutTimer(ID_CONTAINS);

	std::cout << "### multiple ValueStore's from different threads can be added" << std::endl;
	ValueAnalyser ve;
	ve.addValueStore(tsThread1, thread1_id);
	ve.outputToFile();

	return 0;
}
