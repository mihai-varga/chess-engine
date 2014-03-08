#include "chess_board.h"
#include <iostream>
#include <stdio.h>
#include<climits>

using namespace std;

ChessBoard::ChessBoard() {
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
	//initialize piece lookup table
	for (int i = 0; i < 64; i++) {
		ChessBoard::piece[i] = 0;
		ChessBoard::piece[i] = 1LL << i;
	}
	//initialize the board table
	for (int i = 0; i < 12; i++) {
		ChessBoard::boards[i] = 0;
	}
	//pawns
	ChessBoard::boards[0] |= ChessBoard::maskRank[1];
	ChessBoard::boards[6] |= ChessBoard::maskRank[6];
	//rooks
	ChessBoard::boards[1] |= ChessBoard::piece[0] | ChessBoard::piece[7];
	ChessBoard::boards[7] |= ChessBoard::piece[63] | ChessBoard::piece[56];
	//knights
	ChessBoard::boards[2] |= ChessBoard::piece[1] | ChessBoard::piece[6];
	ChessBoard::boards[8] |= ChessBoard::piece[62] | ChessBoard::piece[57];
	//bishops
	ChessBoard::boards[3] |= ChessBoard::piece[2] | ChessBoard::piece[5];
	ChessBoard::boards[9] |= ChessBoard::piece[61] | ChessBoard::piece[58];
	//queen
	ChessBoard::boards[4] |= ChessBoard::piece[3];
	ChessBoard::boards[10] |= ChessBoard::piece[59];
	//king
	ChessBoard::boards[5] |= ChessBoard::piece[4];
	ChessBoard::boards[11] |= ChessBoard::piece[60];
	//initialize global tables
	ChessBoard::allBlacks = 0;
	ChessBoard::allWhites = 0;
	for (int i = 0; i < 6; i++) {
		ChessBoard::allWhites |= ChessBoard::boards[i];
		ChessBoard::allBlacks |= ChessBoard::boards[i + 6];
	}
	ChessBoard::allPieces = ChessBoard::allWhites | ChessBoard::allBlacks;
}

ChessBoard::~ChessBoard() {}

void ChessBoard::printBoard(ChessBoard::Bitboard b) {
	for (int i = 0; i < 64; i++) {
		Bitboard c = b & (1LL << i);
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

ChessBoard::Bitboard ChessBoard::getPiece(char c, int i) {
	if (c > 'h' || c < 'a' || i > 8 || i < 1) throw 1;
	int ind = c - 'a';
	ind += (i - 1) * 8;
	return ChessBoard::piece[ind];
}

/*
 * 0 1 2
 * 3 K 4
 * 5 6 7
 */
ChessBoard::Bitboard ChessBoard::getKingMoves(ChessBoard::Bitboard b) {
	ChessBoard::Bitboard moves[8], ret;
	// trimA and trimH check if the king is on
	// file A or H respectively
	ChessBoard::Bitboard trimA, trimH;
	trimA = b & ChessBoard::clearFile[0];
	trimH = b & ChessBoard::clearFile[7];
	for (int i = 0; i < 8; i++) {
		moves[i] = 0;
	}
	moves[0] = trimA >> 9;
	moves[1] = b >> 8;
	moves[2] = trimH >> 7;
	moves[3] = trimA >> 1;
	moves[4] = trimH << 1;
	moves[5] = trimA << 7;
	moves[6] = b << 8;
	moves[7] = trimH << 9;
	ret = 0;
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

/*
 * - 1 - 2 -
 * 0 - - - 3
 * - - N - -
 * 7 - - - 4
 * - 6 - 5 -
 */
ChessBoard::Bitboard ChessBoard::getKnightMoves(ChessBoard::Bitboard b) {
	ChessBoard::Bitboard moves[8], ret;
	// trimAB and trimGH check if the knight is on
	// files (A, B) or (G, H) respectively
	ChessBoard::Bitboard trimAB, trimGH, trimA, trimH;
	trimAB = b & ChessBoard::clearFile[0] & ChessBoard::clearFile[1];
	trimGH = b & ChessBoard::clearFile[7] & ChessBoard::clearFile[6];
	trimA = b & ChessBoard::clearFile[0];
	trimH = b & ChessBoard::clearFile[7];
	for (int i = 0; i < 8; i++) {
		moves[i] = 0;
	}
	moves[0] = trimAB >> 10;//
	moves[1] = trimA >> 17;//
	moves[2] = trimH >> 15;//
	moves[3] = trimGH >> 6;
	moves[4] = trimGH << 10;
	moves[5] = trimH << 17;
	moves[6] = trimA << 15;
	moves[7] = trimAB << 6;//
	ret = 0;
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

int main() {
	ChessBoard cb;
	cb.printBoard(cb.piece[0]);
	cb.printBoard(cb.piece[1]);
	cb.printBoard(cb.piece[2]);
	cb.printBoard(cb.clearRank[0]);
	cb.printBoard(cb.maskRank[0]);
	cb.printBoard(cb.clearFile[0]);
	cb.printBoard(cb.maskFile[7]);
	cb.printBoard(cb.allWhites);
	cb.printBoard(cb.allBlacks);
	cb.printBoard(cb.allPieces);
	cb.printBoard(cb.getKnightMoves(cb.getPiece('g', 1)));
	return 0;
}
