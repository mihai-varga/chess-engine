#include <stdio.h>
#include <string.h>
#include <map>
#include <csignal>
#include <iostream>
#include "chess_board/chess_board.h"

using namespace std;

bool isMove(char *command) {
    return ((command[0] >= 'a' && command[0] <= 'h') &&
            (command[1] >= '1' && command[1] <= '8') &&
            (command[2] >= 'a' && command[2] <= 'h') &&
            (command[3] >= '1' && command[3] <= '8'));
}

void signal_handler(int signal) {}

void play(ChessBoard &cb) {
    /**
     * Comenzi posibile: winboard, xboard, new, force, go, white, 
     * black, quit, resign, move
     *
     * Xboard poate sa trimita un semnal de intrerupere, il putem prinde
     * cu:  signal(SIGINT, signal_handler)
     * dar putem sa trimitem un msg lui Xboard sa nu mai trimita sigint,
     * daca avem voie, facem asa pentru ca e mai frumos. O sa intrebam pe
     * forum
     * */

    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    //signal(SIGINT, signal_handler);
    
    char command[256];
    bool white = false;
    //bool forceMode = false;

    cb.setCurrentPlayer(cb.BLACK);

    while (1) {
        scanf("%s", command);

        if (!strcmp(command, "xboard")) {
            scanf("%s", command); // protover
            if (!strcmp(command, "protover")) {
                scanf("%s", command); // 2
                printf("feature myname=\"Piept de sahist\"\n");
                printf("feature sigint=0\n"); // nu mai trimite SIGINT 
                printf("feature done=1\n");
                continue;
            }
            else {
                cb.initBoard();
                continue;
            }
        }

        if (!strcmp(command, "new")) {
            cb.initBoard();
            continue;
        }

        if (!strcmp(command, "force")) {
            //forceMode = true;
            continue;
        }

        if (!strcmp(command, "go")) {
            //forceMode = false;
            if (white) {
                cb.setMove(cb.moveToBitboard("a2"), cb.moveToBitboard("a3"));
                printf("move a2a3\n");
            }
            else {
                cb.setMove(cb.moveToBitboard("a7"), cb.moveToBitboard("a6"));
                printf("move a7a6\n");
            }
            continue;
        }

        if (!strcmp(command, "white")) {
            white = true;
            cb.setCurrentPlayer(cb.WHITE);
            continue;
        }

        if (!strcmp(command, "black")) {
            white = false;
            cb.setCurrentPlayer(cb.BLACK);
            continue;
        }

        if (!strcmp(command, "quit")) {
            return;
        }

        if (!strcmp(command, "resign")) {
        }

        if (isMove(command)) {
            std::vector<std::pair<ChessBoard::bitboard_t, int> > attackers;
            cb.setMove(cb.moveToBitboard(command), cb.moveToBitboard(command + 2));
            const char *my_move_const = cb.getNextMoveTmp(command).c_str();
            char *my_move = strdup(my_move_const);
            cb.isCheck(attackers);
            if (attackers.size() == 0 && cb.isValid(cb.moveToBitboard(my_move), cb.moveToBitboard(my_move + 2))) {
            //if (cb.isValid(cb.moveToBitboard(my_move), cb.moveToBitboard(my_move + 2))) {
                cb.setMove(cb.moveToBitboard(my_move), cb.moveToBitboard(my_move + 2));
                printf("move %s\n", my_move);
            }
            else {
                printf("resign\n");
            }

        }
    }
}

int main() {
	ChessBoard cb;
	//play(cb);
	ChessBoard::bitboard_t b = cb.square[7];
//	cb.printBoard(b);
	cb.current_player = cb.WHITE;
	cb.allPieces &= ~cb.square[1];
	cb.allPieces &= ~cb.square[2];
	cb.allPieces &= ~cb.square[3];
	cb.allPieces &= ~cb.square[58];
	cb.allPieces &= ~cb.square[57];
	cb.allPieces &= ~cb.square[59];
	cb.allBlacks &= cb.allPieces;
	cb.allWhites &= cb.allPieces;
	if(cb.isCheck(cb.square[3]).size()>0)
		printf("asd\n");
	printf("printez mv\n");
	cb.printBoard(cb.allPieces);
	cb.printBoard(cb.allWhites);
	cb.printBoard(cb.allBlacks);
	if(cb.canQueensideCastling())
		printf("asd\n");
/*	cb.doKingsideCastling();
	printf("printez mv\n");
	cb.printBoard(cb.allPieces);
	cb.printBoard(cb.allWhites);
	cb.printBoard(cb.allBlacks);
	cb.printBoard(cb.boards[1]);
	cb.printBoard(cb.boards[5]);
	cb.printBoard(cb.boards[7]);
	cb.printBoard(cb.boards[11]);
*/	return 0;      
}

