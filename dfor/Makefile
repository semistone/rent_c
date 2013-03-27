CC = gcc
FLAGS = -I./
CXXFLAGS = -fPIC
#CFLAGS = -fPIC -g $(INCLUDES)
LDFLAGS = -shared
SOURCES= ./DB.cc
OBJECTS= $(SOURCES:.cc=.o)
LINKFLAGS= -lsqlite3
TARGET  = dfor.so
# Default compile
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) $(LINKFLAGS) $(LDFLAGS) -o $(TARGET) $(OBJECTS)

clean:
	rm *.o *.so
