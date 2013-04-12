CC = gcc
#FLAGS = 
CXXFLAGS = -fPIC
#CFLAGS = -fPIC -g $(INCLUDES)
LDFLAGS = -shared -ldl   -lsqlite3 -llog4cpp -fPIC
SOURCES= ./DB.cc ./Hook.cc
OBJECTS= $(SOURCES:.cc=.o)
TARGET  = dfor.so
# Default compile
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

clean:
	rm *.o *.so
install:
	mkdir -p /usr/local/etc/dfor
	mkdir -p /var/run/dfor
	mkdir -p /var/log/dfor
	cp etc/log4cpp.properties /usr/local/etc/dfor
	cp dfor.so /usr/local/lib/
