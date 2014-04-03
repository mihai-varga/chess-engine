#ifndef __CHESS_BOARD_H__
#define __CHESS_BOARD_H__
#include <stdint.h>
#include <vector>
#include <string>

typedef unsigned long long bitboard_t;
typedef enum {WHITE, BLACK} player_t ;
typedef std::pair<bitboard_t, bitboard_t> PairBB;

class ChessBoard {
//	private:
//  public for testing purposes
	public:
		//used for castlings (ro: rocada)
		bool whiteKingMoved, whiteRookKingMoved, whiteRookQueenMoved;
		bool blackKingMoved, blackRookKingMoved, blackRookQueenMoved;
	public:
        /*
         * current player color (no racism involved)
         */
        player_t current_player;
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
		std::vector<bitboard_t> boards;
		bitboard_t allWhites, allBlacks, allPieces;
		/*
		 * rank = row
		 * file = column
		 */
		std::vector<bitboard_t> maskRank;
		std::vector<bitboard_t> clearRank;
		std::vector<bitboard_t> maskFile;
		std::vector<bitboard_t> clearFile;
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
		std::vector<bitboard_t> square;

    private:
        static char alph[8];
        static char numb[8];
    public:
        ChessBoard();
        ~ChessBoard();
        void initBoard();
        void setCurrentPlayer(player_t p);

        /*
         * plots the board given as parameter
         */
		void printBoard(bitboard_t b);

        /*
         * gets the x and y coresponding to bitboard_t b
         */
		std::pair<int, int> getCoords(bitboard_t b);
        /* get the coresponding board
         * @params
         *      b  the piece for which we want 
         *         to determine the type (by picking
         *         the right board that holds a 
         *         certain kind of piece e.g. kings,
         *         pawns etc.)
         * @return the index of the board
         */
        int getBoard(bitboard_t b);

        /*
         * converts a bitboard to "chess format"
         * (e.g. A3, H2 etc.)
         */
        std::string bitboardToMove(bitboard_t b);
        /*
         * converts a "chess format" position on the board
         * to a bitboard
         */
        bitboard_t moveToBitboard(std::string move);

        std::vector<bitboard_t> split(bitboard_t b);

        // updates the board after a new move
        void setMove(bitboard_t from, bitboard_t to);

        // returns the move which the algorithm decides to make next
        std::pair<bitboard_t, bitboard_t> getNextMove ();
        // check if a move is valid
        bool isValid(bitboard_t from, bitboard_t to);
        /*
         * checks if our king is in chess
         */
        void isCheck(std::vector<std::pair<bitboard_t, int> >& attackers);
        void isCheck(std::vector<std::pair<bitboard_t, int> >& attackers, bitboard_t);

		/*
		 * checks for check mate
		 */
        bool isCheckMate();

        /*
         * return a move to get out of chess or 0ULL if checkMate
         */
        std::pair<bitboard_t, bitboard_t> getOutOfCheck(std::vector<std::pair<bitboard_t, int> > &attackers);

        /*
         * returns a vector with all the possible moves
         * a king can make
         */
		void getKingMoves(std::vector<bitboard_t>& moves, bitboard_t b);
        /*
         * returns a bitboard_t with all the possible moves
         * a king can make
         */
		bitboard_t getKingAllMoves(bitboard_t b);
		bitboard_t getKingRandomMove(bitboard_t b);

		bitboard_t getRooksAllMoves(bitboard_t b);
        void getRooksMoves(std::vector<bitboard_t> &moves, bitboard_t b);
        bitboard_t getRooksRandomMove (bitboard_t b);

		bitboard_t getBishopAllMoves(bitboard_t b);
        void getBishopMoves(std::vector<bitboard_t> &moves, bitboard_t b);
        bitboard_t getBishopRandomMove (bitboard_t b);
		
        void getQueenMoves(std::vector<bitboard_t> &moves, bitboard_t b);
        bitboard_t getQueenAllMoves(bitboard_t b);
        bitboard_t getQueenRandomMove (bitboard_t b);

        /*
         * returns a vector with all the possible moves
         * a knight can make
         */
		void getKnightMoves(std::vector<bitboard_t>& moves, bitboard_t b);
        /*
         * returns a bitboard_t with all the possible moves
         * a knight can make
         */
		bitboard_t getKnightAllMoves(bitboard_t b);
		bitboard_t getKnightRandomMove(bitboard_t b);

        /*
         * returns a vector with all the possible moves
         * a white pawn can make
         */
		void getWhitePawnMoves(std::vector<bitboard_t>& moves, bitboard_t b);
        /*
         * returns a bitboard_t with all the possible moves
         * a white pawn can make
         */
		bitboard_t getWhitePawnAllMoves(bitboard_t b);
		bitboard_t getWhitePawnRandomMove(bitboard_t b);

        /*
         * returns a vector with all the possible moves
         * a black pawn can make
         */
		void getBlackPawnMoves(std::vector<bitboard_t>& moves, bitboard_t b);
        /*
         * returns a bitboard_t with all the possible moves
         * a black pawn can make
         */
		bitboard_t getBlackPawnAllMoves(bitboard_t b);
		bitboard_t getBlackPawnRandomMove(bitboard_t b);

        /*
         * Toate mutarile valide posibile: from -> to
         */
        void getAllMoves(std::vector<std::pair<bitboard_t, bitboard_t> > &moves);

		/*
		 * checks if a castling is possible
		 * if so, it does it
		 * TODO implement doCastling
		 */
		bool canKingsideCastling();
		bool canQueensideCastling();
		bool doKingsideCastling();
		bool doQueensideCastling();
};

#endif
