CC = g++
#FLAGS = 
SOURCES= ./client.c
OBJECTS= $(SOURCES:.cc=.o)
TARGET  = client
# Default compile
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) 

clean:
	rm $(TARGET)
