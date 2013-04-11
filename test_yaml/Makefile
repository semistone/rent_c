CC = g++
#FLAGS = 
LDFLAGS = -lyaml-cpp 
SOURCES= ./main.cpp
OBJECTS= $(SOURCES:.cc=.o)
TARGET  = test_yaml
# Default compile
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

clean:
	rm $(TARGET)
