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

/*
 * Checks and sets the castling move
 */
void checkCastling(char *command, ChessBoard &cb) {
    if (!strcmp(command, "e1g1")) {
        cb.setMove(1ULL << 7, 1ULL << 5);
        return;
    }
    if (!strcmp(command, "e1c1")) {
        cb.setMove(1ULL, 1ULL << 3);
        return;
    }
    if (!strcmp(command, "e8g8")) {
        cb.setMove(1ULL << 63, 1ULL << 61);
        return;
    }
    if (!strcmp(command, "e8c8")) {
        cb.setMove(1ULL << 56, 1ULL << 59);
        return;
    }
}



void signal_handler(int signal) {}

void play(ChessBoard &cb) {
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    
    char command[256];
    bool forceMode = false;

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
            forceMode = false;
            cb.initBoard();
            continue;
        }

        if (!strcmp(command, "force")) {
            forceMode = true;
            continue;
        }

        if (!strcmp(command, "go")) {
            forceMode = false;
            pair<bitboard_t, bitboard_t> p = cb.getNextMove();
            string from = cb.bitboardToMove(p.first);
            string to = cb.bitboardToMove(p.second);

            cb.setMove(p.first, p.second);
            printf("move %s\n", (from + to).c_str());
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
            cb.setMove(cb.moveToBitboard(command), cb.moveToBitboard(command + 2));
            checkCastling(command, cb);
            if (forceMode)
                continue;
            pair<bitboard_t, bitboard_t> my_move_bit;

            my_move_bit = cb.getNextMove();

            string from = cb.bitboardToMove(my_move_bit.first);
            string to = cb.bitboardToMove(my_move_bit.second);
            string my_move = from + to;

            if (my_move_bit.first != 0ULL && my_move_bit.second != 0ULL) {
                cb.setMove(my_move_bit.first, my_move_bit.second);
                printf("move %s\n", my_move.c_str());
            } else {
                // it must be Check Mate
                printf("resign\n");
            }
        }
    }
}

int main() {
	ChessBoard cb;
	play(cb);
	return 0;      
}

