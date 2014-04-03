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
    //bool forceMode = false;

    cb.setCurrentPlayer(BLACK);
    vector<pair<bitboard_t, int> > attackers;

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
            if (cb.current_player == WHITE) {
                pair<bitboard_t, bitboard_t> p = cb.getNextMove();
                string from = cb.bitboardToMove(p.first);
                string to = cb.bitboardToMove(p.second);

                cb.setMove(p.first, p.second);
                printf("move %s\n", (from + to).c_str());
            }
        }

        if (!strcmp(command, "white")) {
            cb.setCurrentPlayer(WHITE);
            continue;
        }

        if (!strcmp(command, "black")) {
            cb.setCurrentPlayer(BLACK);
            continue;
        }

        if (!strcmp(command, "quit")) {
            return;
        }

        if (!strcmp(command, "resign")) {
        }

        if (isMove(command)) {
            printf("----------------\n");
            printf("incerc sa fac mutarea %s dar esueaza lamentabil\n", command);
            printf("MUTAREA EFECTIV ESTE:\n");
            cb.printBoard(cb.moveToBitboard(command));
            cb.printBoard(cb.moveToBitboard(command+2));
            printf("Regina este asa:");
            cb.printBoard(cb.boards[4]);
            cb.setMove(cb.moveToBitboard(command), cb.moveToBitboard(command + 2));
            cb.printBoard(cb.boards[4]);
            printf("----------------\n");
            attackers.clear();
            cb.isCheck(attackers);
            pair<bitboard_t, bitboard_t> my_move_bit;
            //cb.printBoard(cb.allPieces);

            if (attackers.size() != 0) {
                // Check
                my_move_bit = cb.getOutOfCheck(attackers);
                printf("am fost in sah\n");
                if (my_move_bit.first == 0ULL) {
                    // check mate
                    printf("resign\n");
                    continue;
                }
            }
            else {
                my_move_bit = cb.getNextMove();
            }

            string from = cb.bitboardToMove(my_move_bit.first);
            string to = cb.bitboardToMove(my_move_bit.second);
            string my_move = from + to;

            if (cb.isValid(cb.moveToBitboard(my_move.c_str()), cb.moveToBitboard(my_move.c_str() + 2))) {
                cb.setMove(my_move_bit.first, my_move_bit.second);
                printf("move %s\n", my_move.c_str());
            } 
            else {
                printf("resign\n");
            }
            //cb.printBoard(cb.allPieces);
        }
    }
}

int main() {
	ChessBoard cb;
	play(cb);
	return 0;      
}

