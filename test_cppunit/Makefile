CXX = g++
INCLUDES= -I./
CXXFLAGS = -g $(INCLUDES)
SRCM= ./CBasicMath.cpp
OBJM = $(SRCM:.cpp=.o)
LINKFLAGS= -lcppunit

testbasicmath: TestBasicMath.cpp $(OBJM)
	$(CXX) $(CXXFLAGS) -o $@ TestBasicMath.cpp $(OBJM) $(LINKFLAGS) $(LINKFLAGSLOG4) $(LIBLOG)

# Default compile

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@
          
