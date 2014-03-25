#include "chess_board.h"
#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <time.h>

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

bool ChessBoard::isValid(ChessBoard::bitboard_t from, ChessBoard::bitboard_t to) {
    if (from == 0 || to == 0) {
        return false;
    }
    // if WHITE moves
    if (current_player == WHITE) {
        if (from & ChessBoard::allWhites) {
            if (to & ChessBoard::allWhites) {
                return false;
            } else {
                return true;
            }
        }
        return false;
    }
    if (current_player == BLACK) {
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

bool ChessBoard::isCheck() {
    ChessBoard::bitboard_t opponentAllMoves = 0ULL;
    ChessBoard::player_t opponentColor = current_player == WHITE ? BLACK : WHITE;
    std::vector<ChessBoard::bitboard_t> aux;
    if (opponentColor == WHITE) {    
        // check king
        opponentAllMoves |= getKingAllMoves(boards[5]); 

        // check queen
        opponentAllMoves |= getQueenAllMoves(boards[4]); 
        
        // check rooks
        aux = split(boards[1]);
        for (unsigned int i = 0; i < aux.size(); i++) {
            opponentAllMoves |= getRooksAllMoves(aux[i]);
        }

        // check knights
        aux = split(boards[2]);
        aux.clear();
        for (unsigned int i = 0; i < aux.size(); i++) {
            opponentAllMoves |= getKnightAllMoves(aux[i]);
        }

        // check bishops
        aux = split(boards[3]);
        aux.clear();
        for (unsigned int i = 0; i < aux.size(); i++) {
            opponentAllMoves |= getBishopAllMoves(aux[i]);
        }

        // check pawns
        aux = split(boards[0]);
        aux.clear();
        for (unsigned int i = 0; i < aux.size(); i++) {
            opponentAllMoves |= getWhitePawnAllMoves(aux[i]);
        }
    } else {
        // check king
        opponentAllMoves |= getKingAllMoves(boards[11]); 

        // check queen
        opponentAllMoves |= getQueenAllMoves(boards[10]); 
        
        // check rooks
        aux = split(boards[7]);
        for (unsigned int i = 0; i < aux.size(); i++) {
            opponentAllMoves |= getRooksAllMoves(aux[i]);
        }

        // check knights
        aux = split(boards[8]);
        aux.clear();
        for (unsigned int i = 0; i < aux.size(); i++) {
            opponentAllMoves |= getKnightAllMoves(aux[i]);
        }

        // check bishops
        aux = split(boards[9]);
        aux.clear();
        for (unsigned int i = 0; i < aux.size(); i++) {
            opponentAllMoves |= getBishopAllMoves(aux[i]);
        }

        // check pawns
        aux = split(boards[6]);
        aux.clear();
        for (unsigned int i = 0; i < aux.size(); i++) {
            opponentAllMoves |= getBlackPawnAllMoves(aux[i]);
        }
    }
    printBoard(opponentAllMoves);
    if (opponentColor == WHITE && ((opponentAllMoves & boards[11]) != 0)) {
        return true;
    }
    if (opponentColor == BLACK && ((opponentAllMoves & boards[5]) != 0)) {
        return true;
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
    bitboard_t currentPlayerPieces = current_player == WHITE ? allWhites : allBlacks;
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

ChessBoard::bitboard_t ChessBoard::getRooksAllMoves (ChessBoard::bitboard_t b){
    std::pair<int, int> initialCoords = ChessBoard::getCoords(b);
    ChessBoard::bitboard_t ret = 0;
    bitboard_t currentPlayerPieces = current_player == WHITE ? allWhites : allBlacks;
    //return (ChessBoard::maskFile[initialCoords.second-1] ^ ChessBoard::maskRank[initialCoords.first-1]);
    int x = initialCoords.first - 1;
    int y = initialCoords.second - 1;
    x--;
    int i = 1;
    while (x >= 0)
    {
        ChessBoard::bitboard_t tmp = b >> (8 * i);
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
            i++;
            x--;
        } else {
            break;
        }
    }
    x = initialCoords.first+1;
    i = 1;
    while (x < 9)
    {
        ChessBoard::bitboard_t tmp = b << (8 * i);
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
            i++;
            x++;
        } else {
            break;
        }
    }
    i = 1;
    while (y > 0)
    {
        ChessBoard::bitboard_t tmp = b >> i;
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
            i++;
            y--;
        } else {
            break;
        }
    }
    y = initialCoords.second+1;
    i = 1;
    while (y < 9)
    {
        ChessBoard::bitboard_t tmp = b << i;
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
            i++;
            y++;
        } else {
            break;
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

ChessBoard::bitboard_t ChessBoard::getBishopAllMoves(ChessBoard::bitboard_t b)
{
    std::pair<int, int> initialCoords = ChessBoard::getCoords(b);
	bitboard_t ret = 0;
    bitboard_t currentPlayerPieces = current_player == WHITE ? allWhites : allBlacks;

	//top,left
	int x = initialCoords.first;
	int y = initialCoords.second;
	int i = 1;
	while(x > 1 && y > 1)
	{
        bitboard_t tmp = b >> (i * 9);
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
            i++;
            x--;
            y--;
        } else {
            break;
        }
	}
	//top,right
	x = initialCoords.first;
	y = initialCoords.second;
	i = 1;
	while(x > 1 && y < 8)
	{
        bitboard_t tmp = b >> (i * 7);
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
            i++;
            x--;
            y++;
        } else {
            break;
        }
	}
	//bot,left
	x = initialCoords.first;
	y = initialCoords.second;
	i = 1;
	while(x < 8 && y > 1)
	{
        bitboard_t tmp = b << (i * 7);
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
            i++;
            x++;
            y--;
        } else {
            break;
        }
	}
	//bot,right
	x = initialCoords.first;
	y = initialCoords.second;
	i = 1;
	while(x < 8 && y < 8)
	{
        bitboard_t tmp = b << (i * 9);
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
            i++;
            x++;
            y++;
        } else {
            break;
        }
	}
	return ret;
}

ChessBoard::bitboard_t ChessBoard::getQueenAllMoves(ChessBoard::bitboard_t b)
{
	bitboard_t rook = getRooksAllMoves(b);
	bitboard_t bishop = getBishopAllMoves(b);
	return rook | bishop;
}

/*
 * - - -
 * - P -
 * - 0 -
 * - 1 -
 */
std::vector<ChessBoard::bitboard_t> ChessBoard::getWhitePawnMoves(ChessBoard::bitboard_t b) {
    std::vector<ChessBoard::bitboard_t> moves;
    moves.push_back(b << 8);
    moves.push_back(b << 16);
    if (ChessBoard::allBlacks & (b << 7))
        moves.push_back(b << 7);
    if (ChessBoard::allBlacks & (b << 9))
        moves.push_back(b << 9);
    return moves;
}
 
ChessBoard::bitboard_t ChessBoard::getWhitePawnAllMoves(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret;
    std::vector<ChessBoard::bitboard_t> moves = getWhitePawnMoves(b);
    if (moves.size() == 2)
        ret = moves[0] | moves[1];
    if (moves.size() == 3)
        ret = moves[0] | moves[1] | moves[2];
    ret &= ~ChessBoard::allWhites;
    return ret;
}
 
ChessBoard::bitboard_t ChessBoard::getWhitePawnRandomMove(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    std::vector<ChessBoard::bitboard_t> moves = getWhitePawnMoves(b);
    srand(time(NULL));
    if (moves.size() == 2)
        ret = moves[rand() % 2];
    if (moves.size() == 3)
        ret = moves[rand() % 3];
    return ret;
}
 
/*
 * - 1 -
 * - 0 -
 * - P -
 * - - -
 */
std::vector<ChessBoard::bitboard_t> ChessBoard::getBlackPawnMoves(ChessBoard::bitboard_t b) {
    std::vector<ChessBoard::bitboard_t> moves;
    moves.push_back(b >> 8);
    moves.push_back(b >> 16);
    if (ChessBoard::allBlacks & (b >> 7))
        moves.push_back(b >> 7);
    if (ChessBoard::allBlacks & (b >> 9))
        moves.push_back(b >> 9);
 
    return moves;
}
 
ChessBoard::bitboard_t ChessBoard::getBlackPawnAllMoves(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret;
    std::vector<ChessBoard::bitboard_t> moves = getBlackPawnMoves(b);
    if (moves.size() == 2)
        ret = moves[0] | moves[1];
    if (moves.size() == 3)
        ret = moves[0] | moves[1] | moves[2];
    ret &= ~ChessBoard::allBlacks;
    return ret;
}
 
ChessBoard::bitboard_t ChessBoard::getBlackPawnRandomMove(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    std::vector<ChessBoard::bitboard_t> moves = getBlackPawnMoves(b);
    srand(time(NULL));
    if (moves.size() == 2)
        ret = moves[rand() % 2];
    if (moves.size() == 3)
        ret = moves[rand() % 3];
    return ret;
}
