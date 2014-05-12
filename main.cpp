#include <stdio.h>
#include <string.h>
#include <map>
#include <csignal>
#include <iostream>
#include "chess_board/chess_board.h"
#include <vector>

using namespace std;

bool isMove(char *command) {
    return ((command[0] >= 'a' && command[0] <= 'h') &&
            (command[1] >= '1' && command[1] <= '8') &&
            (command[2] >= 'a' && command[2] <= 'h') &&
            (command[3] >= '1' && command[3] <= '8'));
}

/*
 * Checks and sets the castling move (only the rook!!!)
 */
void checkCastling(char *command, ChessBoard &cb) {
    bitboard_t piece = cb.moveToBitboard(command);
    if (!strcmp(command, "e1g1") && (piece & cb.boards[5])) {
        cb.setMove(1ULL << 7, 1ULL << 5);
        cb.move_index++;
        return;
    }
    if (!strcmp(command, "e1c1") && (piece & cb.boards[5])) {
        cb.setMove(1ULL, 1ULL << 3);
        cb.move_index++;
        return;
    }
    if (!strcmp(command, "e8g8") && (piece & cb.boards[11])) {
        cb.setMove(1ULL << 63, 1ULL << 61);
        cb.move_index++;
        return;
    }
    if (!strcmp(command, "e8c8") && (piece & cb.boards[11])) {
        cb.setMove(1ULL << 56, 1ULL << 59);
        cb.move_index++;
        return;
    }
}

void play(ChessBoard &cb) {
    int ret;
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    
    char command[256];
    bool forceMode = false;

    cb.setCurrentPlayer(BLACK);
    vector<pair<bitboard_t, int> > attackers;

    while (1) {
        ret = scanf("%s", command);
        if (ret < 0)
            fprintf(stderr, "Command error");

        if (!strcmp(command, "xboard")) {
            ret = scanf("%s", command); // protover
            if (ret < 0)
                fprintf(stderr, "Command error");
            if (!strcmp(command, "protover")) {
                ret = scanf("%s", command); // 2
                if (ret < 0)
                    fprintf(stderr, "Command error");
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
            cb.move_index++;
            printf("move %s\n", (from + to).c_str());
        }

        if (!strcmp(command, "time")) {
            int time_;
            ret = scanf("%d", &time_);
            if (ret < 0)
                cb.remaining_time = 600;
            else
                cb.remaining_time = time_ / 100;
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
            checkCastling(command, cb);
            cb.setMove(cb.moveToBitboard(command), cb.moveToBitboard(command + 2));
            cb.move_index++;
            if (forceMode)
                continue;
            pair<bitboard_t, bitboard_t> my_move_bit;

            my_move_bit = cb.getNextMove();

            string from = cb.bitboardToMove(my_move_bit.first);
            string to = cb.bitboardToMove(my_move_bit.second);
            string my_move = from + to;

            if (my_move_bit.first != 0ULL && my_move_bit.second != 0ULL) {
                cb.setMove(my_move_bit.first, my_move_bit.second);
                cb.move_index++;
                printf("move %s\n", my_move.c_str());
            } else {
                // checks for mate or stalemate
                string result;
                if (cb.isCheck()) 
                {
                    //it is mate
                    if (cb.current_player == WHITE)
                        result = "0-1 {Black mates}";
                    else
                        result = "1-0 {White mates}";
                }
                else
                {
                    //it is stalemate
                    result = "1/2-1/2 {Stalemate}";
                }
                printf("%s\n", result.c_str());
            }
        }
    }
}

int main() {
    ChessBoard cb;
    play(cb);
    return 0;
}
