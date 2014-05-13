CC=g++  
CFLAGS= -g -Wall -std=c++0x -O2
EXE=main
SRC=main.cpp chess_board/*.cpp
DIR=$(shell pwd | sed 's/ /\\ /g')

$(EXE) : $(SRC)
	$(CC) -o $(EXE) $(SRC) $(CFLAGS)

build:
	$(CC) -o $(EXE) $(SRC) $(CFLAGS)

white: $(EXE)
	xboard -debug -fcp $(DIR)/$(EXE) -mode TwoMachines -tc 2

black: $(EXE)
	xboard -debug -scp $(DIR)/$(EXE) -mode TwoMachines -tc 2

hblack: $(EXE)
	xboard -debug -fcp $(DIR)/$(EXE) -mode MachineWhite -tc 2

hwhite: $(EXE)
	xboard -debug -fcp $(DIR)/$(EXE) -tc 10


solomid: $(EXE)
	xboard -debug -fcp $(DIR)/$(EXE) -scp $(DIR)/$(EXE) -mode TwoMachines -tc 10

clean:
	rm $(EXE)
