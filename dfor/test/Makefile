CXX = g++
INCLUDES= -I../
CXXFLAGS = -g $(INCLUDES)
OBJM = $(SRCM:.cpp=.o)
LINKFLAGS= -lcppunit -L../src -ldfor

testdfor:
	ln -s -f ../src/dfor.so libdfor.so
	$(CXX) $(CXXFLAGS) -o $@ TestDB.cpp $(OBJM) $(LINKFLAGS) $(LINKFLAGSLOG4) $(LIBLOG)

clean:
	rm testdfor
	rm libdfor.so
