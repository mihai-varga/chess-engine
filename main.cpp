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
    bool forceMode = false;
    bool my_turn = false;

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
                //initBoard();
                continue;
            }
        }

        if (!strcmp(command, "new")) {
            //initBoard();
            continue;
        }

        if (!strcmp(command, "force")) {
            forceMode = true;
            continue;
        }

        if (!strcmp(command, "go")) {
            forceMode = false;
            ChessBoard::bitboard_t next_move;
            if (white) {
                next_move = cb.getWhitePawnRandomMove(1ULL << 8);
                printf("move %s%s\n", cb.ullToMove(1ULL << 8).c_str(),
                        cb.ullToMove(next_move).c_str());
            }
            else {
                next_move = cb.getBlackPawnRandomMove(1ULL << 56);
                printf("move %s%s\n", cb.ullToMove(1ULL << 56).c_str(), 
                        cb.ullToMove(next_move).c_str());
            }
            printf("resign\n");
            continue;
        }

        if (!strcmp(command, "white")) {
            white = true;
            continue;
        }

        if (!strcmp(command, "black")) {
            white = false;
            continue;
        }

        if (!strcmp(command, "quit")) {
            return;
        }

        if (!strcmp(command, "resign")) {
            return;
        }

        if (isMove(command)) {
            ChessBoard::bitboard_t next_move;
            if (white) {
                next_move = cb.getWhitePawnRandomMove(1ULL << 8);
                printf("move %s%s\n", cb.ullToMove(1ULL << 8).c_str(),
                        cb.ullToMove(next_move).c_str());
            }
            else {
                next_move = cb.getBlackPawnRandomMove(1ULL << 56);
                printf("move %s%s\n", cb.ullToMove(1ULL << 56).c_str(), 
                        cb.ullToMove(next_move).c_str());
            }
            printf("resign\n");
            return;

        }
    }
}

int main() {
    ChessBoard cb;
    play(cb);
    return 0;   
}

