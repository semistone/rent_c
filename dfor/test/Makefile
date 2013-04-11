CXX = g++
INCLUDES= -I../
CXXFLAGS = -g $(INCLUDES)
OBJM = $(SRCM:.cpp=.o)
LINKFLAGS= -lcppunit -L../ -ldfor

testdfor:
	$(CXX) $(CXXFLAGS) -o $@ TestDB.cpp $(OBJM) $(LINKFLAGS) $(LINKFLAGSLOG4) $(LIBLOG)

clean:
	rm testdfor
