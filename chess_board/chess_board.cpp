#include "chess_board.h"
#include <iostream>

using namespace std;

ChessBoard::ChessBoard() {
	for (int i = 0; i < 8; i++) {
		ChessBoard::maskRank[i] = 0;
		ChessBoard::clearRank[i] = -1;
		ChessBoard::maskFile[i] = 0;
		ChessBoard::clearFile[i] = -1;
	}
	for (int i = 0; i < 64; i++) {
		ChessBoard::piece[i] = 0;
		//ChessBoard::piece[i] = 1 << i;
	}
}

ChessBoard::~ChessBoard() {}

void ChessBoard::printBoard(ChessBoard::Bitboard b) {
	for (int i = 0; i < 64; i++) {
		char c = b | (1 << i);
		if (c)
			cout << 1;
		else 
			cout << 0;
	}
	cout << endl;
}

int main() {
	ChessBoard cb;
	cout << sizeof(ChessBoard::Bitboard) << endl;
	cb.printBoard(cb.piece[0]);
	cb.printBoard(cb.piece[1]);
	cb.printBoard(cb.piece[2]);
	cb.printBoard(cb.clearRank[0]);
	cb.printBoard(cb.maskRank[0]);
	return 0;
}
