#include "chess_board.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <time.h>
#include <utility>
#include "piece_square_tables.h"

using namespace std;

char ChessBoard::alph[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
char ChessBoard::numb[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};

void ChessBoard::initBoard() {
    //initialize the board table
    for (int i = 0; i < 12; i++) {
        boards[i] = 0;
    }
    //pawns
    boards[0] |= maskRank[1];
    boards[6] |= maskRank[6];
    //rooks
    boards[1] |= square[0] | square[7];
    boards[7] |= square[63] | square[56];
    //knights
    boards[2] |= square[1] | square[6];
    boards[8] |= square[62] | square[57];
    //bishops
    boards[3] |= square[2] | square[5];
    boards[9] |= square[61] | square[58];
    //queen
    boards[4] |= square[3];
    boards[10] |= square[59];
    //king
    boards[5] |= square[4];
    boards[11] |= square[60];
    //initialize global tables
    allBlacks = 0;
    allWhites = 0;
    for (int i = 0; i < 6; i++) {
        allWhites |= boards[i];
        allBlacks |= boards[i + 6];
    }
    allPieces = allWhites | allBlacks;
    //initialize booleans for castlings
    whiteKingMoved = whiteRookKingMoved = whiteRookQueenMoved = false;
    blackKingMoved = blackRookKingMoved = blackRookQueenMoved = false;

    for (int i = 0; i < 64; i++) {
        indexes[1ULL << i] = i;
    }
}

ChessBoard::ChessBoard() {
    current_player = BLACK;
    boards = vector<bitboard_t>(12, 0);
    maskRank = vector<bitboard_t>(8, 0);
    clearRank = vector<bitboard_t>(8, 0);
    maskFile = vector<bitboard_t>(8, 0);
    clearFile = vector<bitboard_t>(8, 0);
    square = vector<bitboard_t>(64, 0);

    // initialize the masks
    for (int i = 0; i < 8; i++) {
        maskRank[i] = 0;
        clearRank[i] = -1;
        maskFile[i] = 0;
        clearFile[i] = -1;
        for (int j = 8 * i; j < 8 * i + 8; j++) {
            maskRank[i] |= 1LL << j;
            clearRank[i] &= ~(1LL << j);
        }
        for (int j = i; j < 64; j = j + 8) {
            maskFile[i] |= 1LL << j;
            clearFile[i] &= ~(1LL << j);
        }
    }
    //initialize square lookup table
    for (int i = 0; i < 64; i++) {
        square[i] = 1LL << i;
    }

    //set the move index to 1
    move_index = 0;

    initBoard();
}

ChessBoard::~ChessBoard() {}

void ChessBoard::setCurrentPlayer(player_t p) {
    current_player = p;
}

void ChessBoard::printBoard(bitboard_t b) {
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

pair<int, int> ChessBoard::getCoords(bitboard_t b)
{
    //i = line
    //j = column
    int i = 1, j = 1;
    while(1)
    {
        if(b >> 8 == 0) {
            break;
        }
        i++;
        b = b >> 8;
    }
    //mask with 1 on first column
    while(1)
    {
        if(b >> 1 == 0) {
            break;
        }
        j++;
        b = b >> 1;
    }

    pair<int, int> coords = make_pair(i, j);
    return coords;
}

int ChessBoard::getBoard(bitboard_t b) {
    for (int i = 0; i < 12; i++) {
        if (b & boards[i])
            return i;
    }
    return -1;
}

string ChessBoard::bitboardToMove(bitboard_t b) {
    int index = 0;
    while (index < 64 && (b != 1ULL << index)) {
        index++;
    }
    string move;
    move += index == 0 ? alph[0] : alph[index % 8];
    move += numb[index / 8];
    return move;
}

vector<bitboard_t> ChessBoard::split(bitboard_t b) {
    vector<bitboard_t> v;
    while(b)
    {
        v.push_back(b & (-b));
        b &= (b - 1);
    }
    return v;
}

bitboard_t ChessBoard::moveToBitboard(string move) {
    int col = (int)move[0] - 97;
    int row = (int)move[1] - 49;
    return 1ULL << (row * 8 + col);
}

int ChessBoard::evaluate(player_t player) {
    //different move vectors for each color
    vector<pair<bitboard_t, bitboard_t> > white_moves, black_moves;
    vector<bitboard_t> moves;
    int white_moves_size = 1, black_moves_size = 1;
    player_t old = current_player;
    if (isCheck(WHITE)) {
        current_player = WHITE;
        getAllMoves(white_moves);
        white_moves_size = white_moves.size();
    }
    if (isCheck(BLACK)) {
        current_player = BLACK;
        getAllMoves(black_moves);
        black_moves_size = black_moves.size();
    }
    current_player = old;

    vector<bitboard_t> pieces[12];
    int attacked[12];
    for (int i = 0; i < 12; i++) {
        pieces[i] = split(boards[i]);
        attacked[i] = 0;
    }

    //if one player does not have any more moves, it's mate
    if (white_moves_size == 0)
        return player == WHITE ? INT_MIN : INT_MAX;

    if (black_moves_size == 0)
        return player == BLACK ? INT_MIN : INT_MAX;

    int white_pos_score = 0, black_pos_score = 0;

    // WHITE
    for (unsigned int i = 0; i < pieces[0].size(); i++) {
        white_pos_score += pawn_pos[63 - indexes[pieces[0][i]]];
        getWhitePawnMoves(moves, pieces[0][i]);
        for (unsigned int j = 0; j < moves.size(); j++) // all the attack moves
            for (int k = 7; k < 12; k++) // possible targets (without pawns)
                if (moves[j] & boards[k])
                    attacked[k]++;
    }

    for (unsigned int i = 0; i < pieces[1].size(); i++) {
        white_pos_score += rook_pos[63 - indexes[pieces[1][i]]];
        getRooksMoves(moves, pieces[1][i]);
        for (unsigned int j = 0; j < moves.size(); j++)
            for (int k = 7; k < 12; k++)
                if (moves[j] & boards[k])
                    attacked[k]++;
    }

    for (unsigned int i = 0; i < pieces[2].size(); i++) {
        white_pos_score += knight_pos[63 - indexes[pieces[2][i]]];
        getKnightMoves(moves, pieces[2][i]);
        for (unsigned int j = 0; j < moves.size(); j++)
            for (int k = 7; k < 12; k++)
                if (moves[j] & boards[k])
                    attacked[k]++;
    }

    for (unsigned int i = 0; i < pieces[3].size(); i++) {
        white_pos_score += bishop_pos[63 - indexes[pieces[3][i]]];
        getBishopMoves(moves, pieces[3][i]);
        for (unsigned int j = 0; j < moves.size(); j++)
            for (int k = 7; k < 12; k++)
                if (moves[j] & boards[k])
                    attacked[k]++;
    }

    for (unsigned int i = 0; i < pieces[4].size(); i++) {
        white_pos_score += queen_pos[63 - indexes[pieces[4][i]]];
        getQueenMoves(moves, pieces[4][i]);
        for (unsigned int j = 0; j < moves.size(); j++)
            for (int k = 7; k < 12; k++)
                if (moves[j] & boards[k])
                    attacked[k]++;
    }

    for (unsigned int i = 0; i < pieces[5].size(); i++) {
        white_pos_score += king_pos[63 - indexes[pieces[5][i]]];
        getKingMoves(moves, pieces[5][i]);
        for (unsigned int j = 0; j < moves.size(); j++)
            for (int k = 7; k < 12; k++)
                if (moves[j] & boards[k])
                    attacked[k]++;
    }


    // BLACK
    for (unsigned int i = 0; i < pieces[6].size(); i++) {
        black_pos_score += pawn_pos[indexes[pieces[6][i]]];
        getBlackPawnMoves(moves, pieces[6][i]);
        for (unsigned int j = 0; j < moves.size(); j++)
            for (int k = 1; k < 6; k++)
                if (moves[j] & boards[k])
                    attacked[k]++;
    }

    for (unsigned int i = 0; i < pieces[7].size(); i++) {
        black_pos_score += rook_pos[indexes[pieces[7][i]]];
        getRooksMoves(moves, pieces[7][i]);
        for (unsigned int j = 0; j < moves.size(); j++)
            for (int k = 1; k < 6; k++)
                if (moves[j] & boards[k])
                    attacked[k]++;
    }

    for (unsigned int i = 0; i < pieces[8].size(); i++) {
        black_pos_score += knight_pos[indexes[pieces[8][i]]];
        getKnightMoves(moves, pieces[8][i]);
        for (unsigned int j = 0; j < moves.size(); j++)
            for (int k = 1; k < 6; k++)
                if (moves[j] & boards[k])
                    attacked[k]++;
    }

    for (unsigned int i = 0; i < pieces[9].size(); i++) {
        black_pos_score += bishop_pos[indexes[pieces[9][i]]];
        getBishopMoves(moves, pieces[9][i]);
        for (unsigned int j = 0; j < moves.size(); j++)
            for (int k = 1; k < 6; k++)
                if (moves[j] & boards[k])
                    attacked[k]++;
    }

    for (unsigned int i = 0; i < pieces[10].size(); i++) {
        black_pos_score += queen_pos[indexes[pieces[10][i]]];
        getQueenMoves(moves, pieces[10][i]);
        for (unsigned int j = 0; j < moves.size(); j++)
            for (int k = 1; k < 6; k++)
                if (moves[j] & boards[k])
                    attacked[k]++;
    }

    for (unsigned int i = 0; i < pieces[11].size(); i++) {
        black_pos_score += king_pos[indexes[pieces[11][i]]];
        getKingMoves(moves, pieces[11][i]);
        for (unsigned int j = 0; j < moves.size(); j++)
            for (int k = 1; k < 6; k++)
                if (moves[j] & boards[k])
                    attacked[k]++;
    }

    int score = 0;
    
    // Score by the number of pieces
    score += 900 * (pieces[4].size() - pieces[10].size()); // queens
    score += 330 * (pieces[3].size() - pieces[9].size()); // bishops
    score += 320 * (pieces[2].size() - pieces[8].size()); // kights
    score += 500 * (pieces[1].size() - pieces[7].size()); // rooks
    score += 100 * (pieces[0].size() - pieces[6].size()); // pawns
    // Score by the attacked pieces
    score += 200 * (attacked[11] - attacked[5]); 
    score += 90 * (attacked[10] - attacked[4]); // opponent's attacked queen - my attacked queen
    score += 33 * (attacked[9] - attacked[3]);
    score += 32 * (attacked[8] - attacked[2]);
    score += 50 * (attacked[7] - attacked[1]);
    score += 10 * (attacked[6] - attacked[0]);

    if (this->move_index > END_OF_EARLY_GAME)
        score += white_pos_score - black_pos_score;

    // King's Indian Defense (here are evaluated the moves for BLACK)
    if (this->move_index <= END_OF_EARLY_GAME) {
        // black opening
        if (moveToBitboard("f6") & boards[8]) {
            score -= 200;
        }
        if (moveToBitboard("g6") & boards[6]) {
            score -= 200;
        }
        if (moveToBitboard("d5") & boards[6]) {
            score -= 200;
        }
        if (moveToBitboard("g7") & boards[9]) {
            score -= 200;
        }
        if ((moveToBitboard("g8") & boards[11]) && (moveToBitboard("f8") & boards[7])) {
            score -= 200;
        }
        // white opening
        if (moveToBitboard("f3") & boards[2]) {
            score += 200;
        }
        if (moveToBitboard("d2") & boards[2]) {
            score += 200;
        }
        if (moveToBitboard("e4") & boards[0]) {
            score += 200;
        }
        if (moveToBitboard("d3") & boards[0]) {
            score += 200;
        }
        if (moveToBitboard("g3") & boards[0]) {
            score += 200;
        }
        if (moveToBitboard("g2") & boards[3]) {
            score += 200;
        }
        if ((moveToBitboard("g1") & boards[5]) && (moveToBitboard("f2") & boards[1])) {
            score += 200;
        }
    }

    return player == WHITE ? score : -score;
}
