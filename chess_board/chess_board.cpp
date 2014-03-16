#include "chess_board.h"
#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <time.h>

using namespace std;

char ChessBoard::alph[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
char ChessBoard::numb[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};

void ChessBoard::initBoard() {
    //initialize the board table
    for (int i = 0; i < 12; i++) {
        ChessBoard::boards[i] = 0;
    }
    //pawns
    ChessBoard::boards[0] |= ChessBoard::maskRank[1];
    ChessBoard::boards[6] |= ChessBoard::maskRank[6];
    //rooks
    ChessBoard::boards[1] |= ChessBoard::square[0] | ChessBoard::square[7];
    ChessBoard::boards[7] |= ChessBoard::square[63] | ChessBoard::square[56];
    //knights
    ChessBoard::boards[2] |= ChessBoard::square[1] | ChessBoard::square[6];
    ChessBoard::boards[8] |= ChessBoard::square[62] | ChessBoard::square[57];
    //bishops
    ChessBoard::boards[3] |= ChessBoard::square[2] | ChessBoard::square[5];
    ChessBoard::boards[9] |= ChessBoard::square[61] | ChessBoard::square[58];
    //queen
    ChessBoard::boards[4] |= ChessBoard::square[3];
    ChessBoard::boards[10] |= ChessBoard::square[59];
    //king
    ChessBoard::boards[5] |= ChessBoard::square[4];
    ChessBoard::boards[11] |= ChessBoard::square[60];
    //initialize global tables
    ChessBoard::allBlacks = 0;
    ChessBoard::allWhites = 0;
    for (int i = 0; i < 6; i++) {
        ChessBoard::allWhites |= ChessBoard::boards[i];
        ChessBoard::allBlacks |= ChessBoard::boards[i + 6];
    }
    ChessBoard::allPieces = ChessBoard::allWhites | ChessBoard::allBlacks;
}

ChessBoard::ChessBoard() {
    boards = std::vector<ChessBoard::bitboard_t>(12, 0);
    maskRank = std::vector<ChessBoard::bitboard_t>(8, 0);
    clearRank = std::vector<ChessBoard::bitboard_t>(8, 0);
    maskFile = std::vector<ChessBoard::bitboard_t>(8, 0);
    clearFile = std::vector<ChessBoard::bitboard_t>(8, 0);
    square = std::vector<ChessBoard::bitboard_t>(64, 0);

    // initialize the masks
    for (int i = 0; i < 8; i++) {
        ChessBoard::maskRank[i] = 0;
        ChessBoard::clearRank[i] = -1;
        ChessBoard::maskFile[i] = 0;
        ChessBoard::clearFile[i] = -1;
        for (int j = 8 * i; j < 8 * i + 8; j++) {
            ChessBoard::maskRank[i] |= 1LL << j;
            ChessBoard::clearRank[i] &= ~(1LL << j);
        }
        for (int j = i; j < 64; j = j + 8) {
            ChessBoard::maskFile[i] |= 1LL << j;
            ChessBoard::clearFile[i] &= ~(1LL << j);
        }
    }
    //initialize square lookup table
    for (int i = 0; i < 64; i++) {
        ChessBoard::square[i] = 0;
        ChessBoard::square[i] = 1LL << i;
    }
    ChessBoard::initBoard();
}

ChessBoard::~ChessBoard() {}

void ChessBoard::printBoard(ChessBoard::bitboard_t b) {
    for (int i = 0; i < 64; i++) {
        bitboard_t c = b & (1LL << i);
        if (c) {
            cout << 1 << " ";
        } else {
            cout << 0 << " ";
        }
        if ((i + 1) % 8 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

ChessBoard::bitboard_t ChessBoard::getSquare(char c, int i) {
    if (c > 'h' || c < 'a' || i > 8 || i < 1) throw 1;
    int ind = c - 'a';
    ind += (i - 1) * 8;
    return ChessBoard::square[ind];
}

int ChessBoard::getBoard(ChessBoard::bitboard_t b) {
    for (int i = 0; i < 12; i++) {
        if (b & ChessBoard::boards[i])
            return i;
    }
    throw "exception";
}

string ChessBoard::getNextMoveTmp(char *opp_move) {
    int opp_move_int = (int)(opp_move[1] - '0');
    string my_move = "";
    my_move += opp_move[0];
    my_move += numb[(8 - opp_move_int)];
    my_move += opp_move[2];
    opp_move_int = (int)(opp_move[3] - '0');
    my_move += numb[(8 - opp_move_int)];
    return my_move;
}



string ChessBoard::bitboardToMove(ChessBoard::bitboard_t b) {
    int index = 0;
    while (index < 64 && (b != 1ULL << index)) {
        index++;
    }
    string move;
    move += alph[index % 8];
    move += numb[index / 8];
    return move;
}

ChessBoard::bitboard_t ChessBoard::moveToBitboard(string move) {
    int col = (int)move[0] - 97;
    int row = (int)move[1] - 49;
    return 1ULL << (row * 8 + col);
}

void ChessBoard::setMove(ChessBoard::bitboard_t from, ChessBoard::bitboard_t to) {
    int index = ChessBoard::getBoard(from);
    ChessBoard::boards[index] = (ChessBoard::boards[index] & ~from) | to;
    ChessBoard::allPieces = (ChessBoard::allPieces & ~from) | to;
    // if white is moving
    if (from & ChessBoard::allWhites) {
        ChessBoard::allWhites = (ChessBoard::allWhites & ~from) | to;
        if (to & ChessBoard::allBlacks) { // if white attacks black
            int index_black = ChessBoard::getBoard(to);
            ChessBoard::boards[index_black] = ChessBoard::boards[index_black] & ~to;
            //ChessBoard::allPieces = ChessBoard::allPieces & ~to;
            ChessBoard::allBlacks = ChessBoard::allBlacks & ~to;
        }
    } else { // if black is moving
        ChessBoard::allBlacks = (ChessBoard::allBlacks & ~from) | to;
        if (to & ChessBoard::allWhites) { // if black attacks white
            int index_white = ChessBoard::getBoard(to);
            ChessBoard::boards[index_white] = ChessBoard::boards[index_white] & ~to;
            //ChessBoard::allPieces = ChessBoard::allPieces & ~to;
            ChessBoard::allWhites = ChessBoard::allWhites & ~to;
        }
    }
}

bool ChessBoard::isValid(ChessBoard::bitboard_t from, ChessBoard::bitboard_t to,
        ChessBoard::player_t p) {
    if (from == 0 || to == 0) {
        return false;
    }
    // if WHITE moves
    if (p == WHITE) {
        if (from & ChessBoard::allWhites) {
            if (to & ChessBoard::allWhites) {
                return false;
            } else {
                return true;
            }
        }
        return false;
    }
    if (p == BLACK) {
        if (from & ChessBoard::allBlacks) {
            if (to & ChessBoard::allBlacks) {
                return false;
            } else {
                return true;
            }
        }
        return false;
    }
    return false;
}

/*
 * 0 1 2
 * 3 K 4
 * 5 6 7
 */
std::vector<ChessBoard::bitboard_t> ChessBoard::getKingMoves(ChessBoard::bitboard_t b) {
    std::vector<ChessBoard::bitboard_t> moves(8, 0);
    // trimA and trimH check if the king is on
    // file A or H respectively
    ChessBoard::bitboard_t trimA, trimH;
    trimA = b & ChessBoard::clearFile[0];
    trimH = b & ChessBoard::clearFile[7];
    moves[0] = trimA >> 9;
    moves[1] = b >> 8;
    moves[2] = trimH >> 7;
    moves[3] = trimA >> 1;
    moves[4] = trimH << 1;
    moves[5] = trimA << 7;
    moves[6] = b << 8;
    moves[7] = trimH << 9;
    return moves;
}

ChessBoard::bitboard_t ChessBoard::getKingAllMoves(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    std::vector<ChessBoard::bitboard_t> moves = getKingMoves(b);
    for (int i = 0; i < 8; i++) {
        ret |= moves[i];
    }
    // if moving the white king
    if (b == ChessBoard::boards[5]) {
        ret &= ~ChessBoard::allWhites;
    } else {
        if (b == ChessBoard::boards[11]) { // if moving the black king
                ret &= ~ChessBoard::allBlacks;
        } else {
            throw 1;
        }
    }
    return ret;
}

ChessBoard::bitboard_t ChessBoard::getKingRandomMove(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    std::vector<ChessBoard::bitboard_t> moves = getKingMoves(b);
    srand(time(NULL));
    ret = moves[rand() % 8];
    return ret;
}

/*
 * - 1 - 2 -
 * 0 - - - 3
 * - - N - -
 * 7 - - - 4
 * - 6 - 5 -
 */
std::vector<ChessBoard::bitboard_t> ChessBoard::getKnightMoves(ChessBoard::bitboard_t b) {
    std::vector<ChessBoard::bitboard_t> moves(8, 0);
    // trimAB and trimGH check if the knight is on
    // files (A, B) or (G, H) respectively
    ChessBoard::bitboard_t trimAB, trimGH, trimA, trimH;
    trimAB = b & ChessBoard::clearFile[0] & ChessBoard::clearFile[1];
    trimGH = b & ChessBoard::clearFile[7] & ChessBoard::clearFile[6];
    trimA = b & ChessBoard::clearFile[0];
    trimH = b & ChessBoard::clearFile[7];
    moves[0] = trimAB >> 10;
    moves[1] = trimA >> 17;
    moves[2] = trimH >> 15;
    moves[3] = trimGH >> 6;
    moves[4] = trimGH << 10;
    moves[5] = trimH << 17;
    moves[6] = trimA << 15;
    moves[7] = trimAB << 6;
    return moves;
}

ChessBoard::bitboard_t ChessBoard::getKnightAllMoves(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    std::vector<ChessBoard::bitboard_t> moves = getKnightMoves(b);
    for (int i = 0; i < 8; i++) {
        ret |= moves[i];
    }
    // if moving a white knight
    if (b & ChessBoard::boards[2]) {
        ret &= ~ChessBoard::allWhites;
    } else {
        if (b & ChessBoard::boards[8]) { // if moving a black knight
                ret &= ~ChessBoard::allBlacks;
        } else {
            throw 1;
        }
    }
    return ret;
}

ChessBoard::bitboard_t ChessBoard::getKnightRandomMove(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    std::vector<ChessBoard::bitboard_t> moves = getKnightMoves(b);
    srand(time(NULL));
    ret = moves[rand() % 8];
    return ret;
}

/*
 * - - -
 * - P -
 * - 0 -
 * - 1 -
 */
std::vector<ChessBoard::bitboard_t> ChessBoard::getWhitePawnMoves(ChessBoard::bitboard_t b) {
    std::vector<ChessBoard::bitboard_t> moves(2, 0);
    moves[0] = b << 8;
    moves[1] = b << 16;
    return moves;
}

ChessBoard::bitboard_t ChessBoard::getWhitePawnAllMoves(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret;
    std::vector<ChessBoard::bitboard_t> moves = getWhitePawnMoves(b);
    ret = moves[0] | moves[1];
    ret &= ~ChessBoard::allWhites;
    return ret;
}

ChessBoard::bitboard_t ChessBoard::getWhitePawnRandomMove(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    std::vector<ChessBoard::bitboard_t> moves = getWhitePawnMoves(b);
    srand(time(NULL));
    ret = moves[rand() % 2];
    return ret;
}

/*
 * - 1 -
 * - 0 -
 * - P -
 * - - -
 */
std::vector<ChessBoard::bitboard_t> ChessBoard::getBlackPawnMoves(ChessBoard::bitboard_t b) {
    std::vector<ChessBoard::bitboard_t> moves(2, 0);
    moves[0] = b >> 8;
    moves[1] = b >> 16;
    return moves;
}

ChessBoard::bitboard_t ChessBoard::getBlackPawnAllMoves(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret;
    std::vector<ChessBoard::bitboard_t> moves = getBlackPawnMoves(b);
    ret = moves[0] | moves[1];
    ret &= ~ChessBoard::allBlacks;
    return ret;
}

ChessBoard::bitboard_t ChessBoard::getBlackPawnRandomMove(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    std::vector<ChessBoard::bitboard_t> moves = getBlackPawnMoves(b);
    srand(time(NULL));
    ret = moves[rand() % 2];
    return ret;
}
