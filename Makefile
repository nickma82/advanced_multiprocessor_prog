CXXFLAGS= -O3 -Wall -std=c++11 -m64
CPPFLAGS= $(INC_PARAMS)

BENCHMARKOBJS =		src/main.o

TESTOBJS = 	kpozniak/test.o

OBJS =  benchmarker/src/TimeMeasurement.o \
		benchmarker/src/TimeValues.o \
		benchmarker/src/ValueAnalyser.o \
		benchmarker/src/ValueStore.o \
		src_argparser/src/cmdlineOptions.o \
		kpozniak/AMPReferenceSet.o \
		kpozniak/FineGrainedLockingSet.o \
		kpozniak/OptimisticSynchronizationSet.o \
		kpozniak/LazySynchronizationSet.o \
		kpozniak/LockFreeSet.o 
		
LIBS = -pthread -m64

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    LIBS = -mmacosx-version-min=10.6
    CXX = g++
endif

INCLUDE_DIR = benchmarker/src/ kpozniak/ src_argparser/src/
# creating the include directory parameter list
# @see http://stackoverflow.com/questions/4134764/how-to-define-several-include-path-in-makefile
INC_PARAMS=$(foreach d, $(INCLUDE_DIR), -I$d)



TARGET =	ampp_root
TESTTARGET = ampp_test

all:	benchmark test

benchmark: $(OBJS) $(BENCHMARKOBJS)
	$(CXX) -o $(TARGET) $(BENCHMARKOBJS) $(OBJS) $(LIBS)

test:  $(OBJS) $(TESTOBJS)
	$(CXX) -o $(TESTTARGET) $(TESTOBJS) $(OBJS) $(LIBS)	

clean:
	rm -f $(OBJS) $(TARGET) $(TESTTARGET) $(BENCHMARKOBJS) $(TESTOBJS)
