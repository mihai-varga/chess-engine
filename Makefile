CC=g++
CFLAGS= -g -Wall
EXE=main
SRC=main.cpp chess_board/*.cpp
DIR=$(shell pwd | sed 's/ /\\ /g')

$(EXE) : $(SRC)
	$(CC) -o $(EXE) $(SRC) $(CFLAGS)

build:
	$(CC) -o $(EXE) $(SRC) $(CFLAGS)

white: $(EXE)
	xboard -debug -fcp $(DIR)/$(EXE) -mode TwoMachines

black: $(EXE)
	xboard -debug -scp $(DIR)/$(EXE) -mode TwoMachines

hblack: $(EXE)
	xboard -debug -fcp $(DIR)/$(EXE) -mode MachineWhite

hwhite: $(EXE)
	xboard -debug -fcp $(DIR)/$(EXE)


solomid: $(EXE)
	xboard -debug -fcp $(DIR)/$(EXE) -scp $(DIR)/$(EXE) -mode TwoMachines

clean:
	rm $(EXE)
