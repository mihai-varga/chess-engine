#ifndef __CHESS_BOARD_H__
#define __CHESS_BOARD_H__

class ChessBoard {
	public:
		typedef long long Bitboard;
	public:
		/*
		 *  0 - white pawns
		 *  1 - white rooks
		 *  2 - white horses
		 *  3 - white bishops
		 *  4 - white queen
		 *  5 - white king
		 *  __________________
		 *
		 *  6 - black pawns
		 *  7 - black rooks
		 *  8 - black horses
		 *  9 - black bishops
		 * 10 - black queen
		 * 11 - black king
		 */
		Bitboard boards[12], allWhites, allBlacks, allPieces;
		/*
		 * rank = row
		 * file = column
		 */
		Bitboard maskRank[8], clearRank[8], maskFile[8], clearFile[8];
		Bitboard piece[64];

	public:
		ChessBoard();
		~ChessBoard();

		void printBoard(Bitboard b);
};

#endif
