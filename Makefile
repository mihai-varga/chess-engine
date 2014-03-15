CC=g++
CFLAGS= -g -Wall
EXE=main
SRC=main.cpp chess_board/chess_board.cpp
DIR=$(shell pwd | sed 's/ /\\ /g')

$(EXE) : $(SRC)
	$(CC) -o $(EXE) $(SRC) $(CFLAGS)


run: $(EXE)
	xboard -debug  -fcp $(DIR)/$(EXE)  -mode TwoMachines

black: $(EXE)
	xboard -debug  -scp $(DIR)/$(EXE)  -mode TwoMachines

