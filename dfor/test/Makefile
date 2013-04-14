CXX = g++
INCLUDES= -I../
CXXFLAGS = -g $(INCLUDES)
OBJM = $(SRCM:.cpp=.o)
LINKFLAGS= -lcppunit -L../src -ldfor -lsqlite3 -llog4cpp

testdfor:
	ln -s -f ../src/dfor.so libdfor.so
	$(CXX) $(CXXFLAGS) -o $@ TestDB.cpp $(OBJM) $(LINKFLAGS) $(LINKFLAGSLOG4) $(LIBLOG)

clean:
	rm -f testdfor
	rm -f *.so
	rm -f *.db
	rm -f *.xml
