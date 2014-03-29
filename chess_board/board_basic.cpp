#include "chess_board.h"
#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <time.h>
#include <utility>

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
	//initialize booleans for castlings
	whiteKingMoved = whiteRookKingMoved = whiteRookQueenMoved = false;
	blackKingMoved = blackRookKingMoved = blackRookQueenMoved = false;
}

ChessBoard::ChessBoard() {
    current_player = BLACK;
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
//        ChessBoard::square[i] = 0;
        ChessBoard::square[i] = 1LL << i;
    }
    ChessBoard::initBoard();
}

ChessBoard::~ChessBoard() {}

void ChessBoard::setCurrentPlayer(ChessBoard::player_t p) {
    ChessBoard::current_player = p;
}

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

std::pair<int, int> ChessBoard::getCoords(ChessBoard::bitboard_t b)
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

	std::pair<int, int> coords = std::make_pair(i, j);
	return coords;
}

int ChessBoard::getBoard(ChessBoard::bitboard_t b) {
    for (int i = 0; i < 12; i++) {
        if (b & ChessBoard::boards[i])
            return i;
    }
    throw "exception";
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

std::vector<ChessBoard::bitboard_t> ChessBoard::split(ChessBoard::bitboard_t b) {
    std::vector<ChessBoard::bitboard_t> v;
    //while(b)
	//{
	//	v.push_back(b & (-b));
	//	b &= (b - 1);
	//}
    for (int i = 0; i < 64; i++) {
        if (square[i] & b) {
            v.push_back(square[i]);
        }
    }

	return v;
}

ChessBoard::bitboard_t ChessBoard::moveToBitboard(string move) {
    int col = (int)move[0] - 97;
    int row = (int)move[1] - 49;
    return 1ULL << (row * 8 + col);
}
