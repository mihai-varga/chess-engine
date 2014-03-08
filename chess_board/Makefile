CC=g++
CFLAGS=-c
LDFLAGS=

SOURCES=Action.cpp CaicWriter.cpp discretematrix.cpp \
	   MesaConsoleApp.cpp nexusblock.cpp QueueElement.cpp TabReader.cpp

OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=mesa

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	        $(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	        $(CC) $(CFLAGS) $< -o $@

clean:
	        rm -rf *.o
