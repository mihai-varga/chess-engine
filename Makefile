CC=g++
CFLAGS= -g -Wall
EXE=main
SRC=main.cpp chess_board/chess_board.cpp
DIR=$(shell pwd)

$(EXE) : $(SRC)
	$(CC) -o $(EXE) $(SRC) $(CFLAGS)


run: $(EXE)
	xboard -debug  -fcp $(DIR)/$(EXE)  -mode TwoMachines

