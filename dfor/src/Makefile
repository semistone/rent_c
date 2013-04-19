CC = gcc
#FLAGS = 
CXXFLAGS = -fPIC
#CFLAGS = -fPIC -g $(INCLUDES)
LDFLAGS = -shared -ldl -llog4cpp -fPIC
SOURCES= ./Hook.cc
OBJECTS= $(SOURCES:.cc=.o)
TARGET  = dfor.so
# Default compile
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(FLAGS) $(CFLAGS) -o $(TARGET) $(OBJECTS)  $(LDFLAGS)
	ln -s -f dfor.so libdfor.so

clean:
	rm *.o *.so

