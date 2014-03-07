CC=g++
CFLAGS= -g -Wall
OBJ=communicate.o
EXE=communicate
SRC=communicate.cpp
DIR=$(shell pwd)

$(EXE) : $(SRC)
	$(CC) -o $(EXE) $(SRC) $(CFLAGS)


run: $(EXE)
	xboard -debug  -fcp $(DIR)/$(EXE)  -mode TwoMachines

