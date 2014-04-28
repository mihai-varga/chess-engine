#include "chess_board.h"
#include <iostream>
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
//        square[i] = 0;
        square[i] = 1LL << i;
    }
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
    if (player == WHITE)
        return evaluate_white();
    else
        return evaluate_black();
    /*
    player_t old = current_player;
    vector<pair<bitboard_t, bitboard_t> > white_moves, black_moves;
    current_player = WHITE;
    getAllMoves(white_moves);
    current_player = BLACK;
    getAllMoves(black_moves);
    current_player = old;

    vector<bitboard_t> pieces[12];
    for (int i = 0; i < 12; i++)
        pieces[i] = split(boards[i]);

    if ((player == WHITE && white_moves.size() == 0)
            || (player == BLACK && black_moves.size() == 0))
        return INT_MIN;

    if ((player == WHITE && black_moves.size() == 0)
            || (player == BLACK && white_moves.size() == 0))
        return INT_MAX;

    int white_pos_score = 0, black_pos_score = 0;

    for (unsigned int i = 0; i < pieces[0].size(); i++)
        white_pos_score += pawn_pos[63 - indexes[pieces[0][i]]];
    for (unsigned int i = 0; i < pieces[1].size(); i++)
        white_pos_score += rook_pos[63 - indexes[pieces[1][i]]];
    for (unsigned int i = 0; i < pieces[2].size(); i++)
        white_pos_score += knight_pos[63 - indexes[pieces[2][i]]];
    for (unsigned int i = 0; i < pieces[3].size(); i++)
        white_pos_score += bishop_pos[63 - indexes[pieces[3][i]]];
    for (unsigned int i = 0; i < pieces[4].size(); i++)
        white_pos_score += queen_pos[63 - indexes[pieces[4][i]]];
    for (unsigned int i = 0; i < pieces[5].size(); i++)
        white_pos_score += king_pos[63 - indexes[pieces[5][i]]];

    for (unsigned int i = 0; i < pieces[6].size(); i++)
        black_pos_score += pawn_pos[indexes[pieces[6][i]]];
    for (unsigned int i = 0; i < pieces[7].size(); i++)
        black_pos_score += rook_pos[indexes[pieces[7][i]]];
    for (unsigned int i = 0; i < pieces[8].size(); i++)
        black_pos_score += knight_pos[indexes[pieces[8][i]]];
    for (unsigned int i = 0; i < pieces[9].size(); i++)
        black_pos_score += bishop_pos[indexes[pieces[9][i]]];
    for (unsigned int i = 0; i < pieces[10].size(); i++)
        black_pos_score += queen_pos[indexes[pieces[10][i]]];
    for (unsigned int i = 0; i < pieces[11].size(); i++)
        black_pos_score += king_pos[indexes[pieces[11][i]]];


    int score = 0;
    score += 900 * (pieces[4].size() - pieces[10].size()); // queens
    score += 330 * (pieces[3].size() - pieces[9].size()); // bishops
    score += 320 * (pieces[2].size() - pieces[8].size()); // kights
    score += 500 * (pieces[1].size() - pieces[7].size()); // rooks
    score += 100 * (pieces[0].size() - pieces[6].size()); // pawns
    score += 10 * (white_moves.size() - black_moves.size());
    score += white_pos_score - black_pos_score;
    cout << "scor alb " << white_pos_score << "\nscor niggas" << black_pos_score << "\n";
    
    score = player == WHITE ? score : -score;
    return score;*/
}

int ChessBoard::evaluate_white ()
{
    vector<pair<bitboard_t, bitboard_t> > white_moves, black_moves;
    getAllMoves(white_moves);
    current_player = BLACK;
    getAllMoves(black_moves);
    current_player = WHITE;

    vector<bitboard_t> pieces[12];
    for (int i = 0; i < 12; i++)
        pieces[i] = split(boards[i]);

    if (white_moves.size() == 0)
        return INT_MIN;

    if (black_moves.size() == 0)
        return INT_MAX;

    int white_pos_score = 0, black_pos_score = 0;

    for (unsigned int i = 0; i < pieces[0].size(); i++)
        white_pos_score += white_pawn_pos[63 - indexes[pieces[0][i]]];
    for (unsigned int i = 0; i < pieces[1].size(); i++)
        white_pos_score += white_rook_pos[63 - indexes[pieces[1][i]]];
    for (unsigned int i = 0; i < pieces[2].size(); i++)
        white_pos_score += white_knight_pos[63 - indexes[pieces[2][i]]];
    for (unsigned int i = 0; i < pieces[3].size(); i++)
        white_pos_score += white_bishop_pos[63 - indexes[pieces[3][i]]];
    for (unsigned int i = 0; i < pieces[4].size(); i++)
        white_pos_score += white_queen_pos[63 - indexes[pieces[4][i]]];
    for (unsigned int i = 0; i < pieces[5].size(); i++)
        white_pos_score += white_king_pos[63 - indexes[pieces[5][i]]];

    for (unsigned int i = 0; i < pieces[6].size(); i++)
        black_pos_score += black_pawn_pos[indexes[pieces[6][i]]];
    for (unsigned int i = 0; i < pieces[7].size(); i++)
        black_pos_score += black_rook_pos[indexes[pieces[7][i]]];
    for (unsigned int i = 0; i < pieces[8].size(); i++)
        black_pos_score += black_knight_pos[indexes[pieces[8][i]]];
    for (unsigned int i = 0; i < pieces[9].size(); i++)
        black_pos_score += black_bishop_pos[indexes[pieces[9][i]]];
    for (unsigned int i = 0; i < pieces[10].size(); i++)
        black_pos_score += black_queen_pos[indexes[pieces[10][i]]];
    for (unsigned int i = 0; i < pieces[11].size(); i++)
        black_pos_score += black_king_pos[indexes[pieces[11][i]]];


    int score = 0;
    score += 900 * (pieces[4].size() - pieces[10].size()); // queens
    score += 330 * (pieces[3].size() - pieces[9].size()); // bishops
    score += 320 * (pieces[2].size() - pieces[8].size()); // kights
    score += 500 * (pieces[1].size() - pieces[7].size()); // rooks
    score += 100 * (pieces[0].size() - pieces[6].size()); // pawns
    score += 10 * (white_moves.size() - black_moves.size());
    score += white_pos_score - black_pos_score;
    
    return score;
}

int ChessBoard::evaluate_black()
{
    vector<pair<bitboard_t, bitboard_t> > white_moves, black_moves;
    getAllMoves(black_moves);
    current_player = WHITE;
    getAllMoves(white_moves);
    current_player = BLACK;

    vector<bitboard_t> pieces[12];
    for (int i = 0; i < 12; i++)
        pieces[i] = split(boards[i]);

    if (black_moves.size() == 0)
        return INT_MIN;

    if (white_moves.size() == 0)
        return INT_MAX;

    int white_pos_score = 0, black_pos_score = 0;

    for (unsigned int i = 0; i < pieces[0].size(); i++)
        white_pos_score += white_pawn_pos[63 - indexes[pieces[0][i]]];
    for (unsigned int i = 0; i < pieces[1].size(); i++)
        white_pos_score += white_rook_pos[63 - indexes[pieces[1][i]]];
    for (unsigned int i = 0; i < pieces[2].size(); i++)
        white_pos_score += white_knight_pos[63 - indexes[pieces[2][i]]];
    for (unsigned int i = 0; i < pieces[3].size(); i++)
        white_pos_score += white_bishop_pos[63 - indexes[pieces[3][i]]];
    for (unsigned int i = 0; i < pieces[4].size(); i++)
        white_pos_score += white_queen_pos[63 - indexes[pieces[4][i]]];
    for (unsigned int i = 0; i < pieces[5].size(); i++)
        white_pos_score += white_king_pos[63 - indexes[pieces[5][i]]];

    for (unsigned int i = 0; i < pieces[6].size(); i++)
        black_pos_score += black_pawn_pos[indexes[pieces[6][i]]];
    for (unsigned int i = 0; i < pieces[7].size(); i++)
        black_pos_score += black_rook_pos[indexes[pieces[7][i]]];
    for (unsigned int i = 0; i < pieces[8].size(); i++)
        black_pos_score += black_knight_pos[indexes[pieces[8][i]]];
    for (unsigned int i = 0; i < pieces[9].size(); i++)
        black_pos_score += black_bishop_pos[indexes[pieces[9][i]]];
    for (unsigned int i = 0; i < pieces[10].size(); i++)
        black_pos_score += black_queen_pos[indexes[pieces[10][i]]];
    for (unsigned int i = 0; i < pieces[11].size(); i++)
        black_pos_score += black_king_pos[indexes[pieces[11][i]]];


    int score = 0;
    score += 900 * (pieces[10].size() - pieces[4].size()); // queens
    score += 330 * (pieces[9].size() - pieces[3].size()); // bishops
    score += 320 * (pieces[8].size() - pieces[2].size()); // kights
    score += 500 * (pieces[7].size() - pieces[1].size()); // rooks
    score += 100 * (pieces[6].size() - pieces[0].size()); // pawns
    score += 10 * (black_moves.size() - white_moves.size());
    score += black_pos_score - white_pos_score;
    
    return score;
}
