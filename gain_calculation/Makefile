EXENAME = cal_gain 
OBJS    = cal_gain.o

AR            = g++  
CXX           = g++ 
ROOTINCS      = $(shell root-config --cflags)
ROOTLIBS      = $(shell root-config --glibs) -lMinuit -L/usr/lib/libgfortran.so.1.0.0 -lSpectrum 
CXXFLAGS      = -g -Wall -fPIC $(ROOTINCS)
SOFLAGS       = -shared

all: $(OBJS)
	$(CXX) -o $(EXENAME) $(OBJS) $(ROOTLIBS)

%.o:%.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o:%.C
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o:%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(EXENAME) *~
