CC = gcc
#FLAGS = 
CXXFLAGS = -fPIC
#CFLAGS = -fPIC -g $(INCLUDES)
LDFLAGS = -shared -ldl   -lsqlite3 -llog4cpp -fPIC
SOURCES= ./DB.cc ./Hook.cc
OBJECTS= $(SOURCES:.cc=.o)
TARGET  = libdfor.so
# Default compile
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

clean:
	rm *.o *.so
