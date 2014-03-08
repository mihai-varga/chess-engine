CC=g++
CFLAGS=-c
LDFLAGS=

OBJECTS=$(SOURCES:.cpp=.o)

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	        $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	        $(CC) $(CFLAGS) $< -o $@

clean:
	        rm -rf *.o
