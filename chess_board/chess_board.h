#ifndef __CHESS_BOARD_H__
#define __CHESS_BOARD_H__
#include <stdint.h>
#include <vector>

class ChessBoard {
	public:
		typedef unsigned long long Bitboard;
	public:
		/*
		 *  0 - white pawns
		 *  1 - white rooks
		 *  2 - white knights
		 *  3 - white bishops
		 *  4 - white queen
		 *  5 - white king
		 *  __________________
		 *
		 *  6 - black pawns
		 *  7 - black rooks
		 *  8 - black knights
		 *  9 - black bishops
		 * 10 - black queen
		 * 11 - black king
		 */
		std::vector<Bitboard> boards(12, 0);
		Bitboard allWhites, allBlacks, allPieces;
		/*
		 * rank = row
		 * file = column
		 */
		std::vector<Bitboard> maskRank(8, 0), clearRank(8, 0), maskFile(8, 0),
			clearFile(8, 0);
		/*
		 * A1 B1 C1 D1 E1 F1 G1 H1
		 * A2 B2 C2 D2 E2 F2 G2 H2
		 * A3 B3 C3 D3 E3 F3 G3 H3
		 * A4 B4 C4 D4 E4 F4 G4 H4
		 * A5 B5 C5 D5 E5 F5 G5 H5
		 * A6 B6 C6 D6 E6 F6 G6 H6
		 * A7 B7 C7 D7 E7 F7 G7 H7
		 * A8 B8 C8 D8 E8 F8 G8 H8
		 */
		std::vector<Bitboard> piece(64, 0);

	private:
		void initBoard();
	public:
		ChessBoard();
		~ChessBoard();

		void printBoard(Bitboard b);
		Bitboard getPiece(char c, int i);
		std::vector<Bitboard> getKingMoves(Bitboard b);
		Bitboard getKingAllMoves(Bitboard b);
		std::vector<Bitboard> getKnightMoves(Bitboard b);
		Bitboard getKnightAllMoves(Bitboard b);
		std::vector>Bitboard> getWhitePawnMoves(Bitboard b);
		Bitboard getWhitePawnAllMoves(Bitboard b);
		std::vector>Bitboard> getBlackPawnMoves(Bitboard b);
		Bitboard getBlackPawnAllMoves(Bitboard b);
};

#endif
