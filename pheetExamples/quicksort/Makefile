OPTIMIZATION = -O0 -g
CXXFLAGS = $(OPTIMIZATION) -Wall -fmessage-length=0 -std=c++11
CXX = g++

LIB_PATH =
INCLUDE_PATH = -I../../../pheet/

LIBS = -lpthread -lhwloc
	
#all: sequential parallel
all: parallel

clean:
	rm sequential parallel
	
%:	%.cpp
	$(CXX) $@.cpp -o $@ $(CXXFLAGS) $(INCLUDE_PATH) $(LIB_PATH) $(LIBS)
