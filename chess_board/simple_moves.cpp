#include "chess_board.h"
#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <time.h>

#include <stdio.h>

using namespace std;

void ChessBoard::setMove(bitboard_t from, bitboard_t to) {
    int index = getBoard(from);
    int index_opponent = getBoard(to);
    //printBoard(boards[4]);
    boards[index] = (boards[index] & ~from) | to;
    allPieces = (allPieces & ~from) | to;
    // if white is moving
    if (from & allWhites) {
        if (index == 0) { // if moving a pawn 
            if (to & maskRank[7]) { // if the pawn has reached the last rank
                boards[index] = boards[index] & ~to; // delete the pawn
                boards[4] = boards[4] | to; // add the queen
            }
        }
        allWhites = (allWhites & ~from) | to;
        //if (to & allBlacks) { // if white attacks black
        if (index_opponent >= 0) { // if white attacks black
            boards[index_opponent] = boards[index_opponent] & ~to;
            allBlacks = allBlacks & ~to;
            //printBoard(boards[4]);
        }
    } else { // if black is moving
        if (index == 6) { // if moving a pawn 
            if (to & maskRank[0]) { // if the pawn has reached the last rank
                boards[index] = boards[index] & ~to; // delete the pawn
                boards[10] = boards[10] | to; // add the queen
            }
        }
        allBlacks = (allBlacks & ~from) | to;
        //if (to & allWhites) { // if black attacks white
        if (index_opponent >= 0) { // if black attacks white 
            boards[index_opponent] = boards[index_opponent] & ~to;
            allWhites = allWhites & ~to;
        }
    }
}

pair<bitboard_t, bitboard_t> ChessBoard::getNextMove()
{
    vector<pair<bitboard_t, bitboard_t> > all_moves;
    getAllMoves(all_moves);
    if (all_moves.size() == 0)
        return make_pair(0ULL, 0ULL);

    // 10 attempts to make a random move
    srand(time(NULL));
    for (int i = 0; i < 10; i++) {
        int rand_move = rand() % all_moves.size();
        if (isValid(all_moves[rand_move].first, all_moves[rand_move].second)) {
            return all_moves[rand_move];
        }
    }

    // iterate over all moves
    for (unsigned int i = 0; i < all_moves.size(); i++) {
        if (isValid(all_moves[i].first, all_moves[i].second)) {
            return all_moves[i];
        }
    }
    return make_pair(0ULL, 0ULL);
}

bool ChessBoard::isValid(bitboard_t from, bitboard_t to) {
    std::vector<std::pair<bitboard_t, int> > attackers;
    //printBoard(from);
    //printBoard(to);
    if (from == 0 || to == 0) {
        return false;
    }
    // if WHITE moves
    if (current_player == WHITE) {
        // check if the king will be in check
        if (from & boards[5]) {
            isCheck(attackers, to);
            if (attackers.size() != 0)
                return false;
        }
        if (from & allWhites) {
            if (to & allWhites) {
                return false;
            } else {
                return true;
            }
        }
        return false;
    }
    // if BLACK moves
    if (current_player == BLACK) {
        // check if the king will be in check
        if (from & boards[11]) {
            isCheck(attackers, to);
            if (attackers.size() != 0)
                return false;
        }
        if (from & allBlacks) {
            if (to & allBlacks) {
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
 * checks if current's player king is in check
 *
 * ATTENTION !!!!
 * if king != 0 it checks from that position assuming it is the current player's king
 * TODO implement to check if king is not the current player's king
 * TODO implement isCheck using rays
 */
void ChessBoard::isCheck(vector< pair<bitboard_t, int> >& attackers,
                                 bitboard_t king) {
    //bitboard_t opponentAllMoves = 0ULL;
    player_t opponentColor = current_player == WHITE ? BLACK : WHITE;
    vector<bitboard_t> aux;
    attackers.clear();
    // attacker position  &  attacker bitboard index
	if(king == 0ULL)
	{
		if (current_player == BLACK)
		{
			//set to black king
			king = boards[11];
		}
		else
		{
			//set to white king
			king = boards[5];
		}
	}
    if (opponentColor == WHITE) {    
        // check queen
        //printBoard(king);
        //printBoard(boards[4]);
            cout << "coaie" << endl;
            printBoard(getQueenAllMoves(boards[4]));
        if (king & getQueenAllMoves(boards[4])) {
            attackers.push_back(make_pair(boards[4], 4));
        }
        
        // check rooks
        aux = split(boards[1]);
        for (unsigned int i = 0; i < aux.size(); i++) {
            if (king & getRooksAllMoves(aux[i])) {
                attackers.push_back(make_pair(aux[i], 1));
            }
        }

        // check knights
        aux = split(boards[2]);
        for (unsigned int i = 0; i < aux.size(); i++) {
            if (king & getKnightAllMoves(aux[i])) {
                attackers.push_back(make_pair(aux[i], 2));
            }
        }

        // check bishops
        aux = split(boards[3]);
        for (unsigned int i = 0; i < aux.size(); i++) {
            if (king & getBishopAllMoves(aux[i])) {
                attackers.push_back(make_pair(aux[i], 3));
            }
        }

        // check pawns
        aux = split(boards[0]);
        for (unsigned int i = 0; i < aux.size(); i++) {
            if (king & getWhitePawnAllMoves(aux[i])) {
                attackers.push_back(make_pair(aux[i], 0));
            }
        }
        //printBoard(allOpponentMoves);
    } else {
        // check queen
        if (king & getQueenAllMoves(boards[10])) {
            bitboard_t queen_pos = split(boards[10])[0];
            attackers.push_back(make_pair(queen_pos, 10));
        }
        
        // check rooks
        aux = split(boards[7]);
        for (unsigned int i = 0; i < aux.size(); i++) {
            if (king & getRooksAllMoves(aux[i])) {
                attackers.push_back(make_pair(aux[i], 7));
            }
        }

        // check knights
        aux = split(boards[8]);
        for (unsigned int i = 0; i < aux.size(); i++) {
            if (king & getKnightAllMoves(aux[i])) {
                attackers.push_back(make_pair(aux[i], 8));
            }
        }

        // check bishops
        aux = split(boards[9]);
        for (unsigned int i = 0; i < aux.size(); i++) {
            if (king & getBishopAllMoves(aux[i])) {
                attackers.push_back(make_pair(aux[i], 9));
            }
        }

        // check pawns
        aux = split(boards[6]);
        for (unsigned int i = 0; i < aux.size(); i++) {
            if (king & getBlackPawnAllMoves(aux[i])) {
                attackers.push_back(make_pair(aux[i], 6));
            }
        }
    }
}

void ChessBoard::isCheck(vector< pair<bitboard_t, int> >& attackers) {
	this->isCheck(attackers, 0ULL);
}

bool ChessBoard::isCheckMate() {

    return false;
}

/*
 * 0 1 2
 * 3 K 4
 * 5 6 7
 */
void ChessBoard::getKingMoves(vector<bitboard_t>& moves, bitboard_t b) {
    vector<bitboard_t> aux_moves(8, 0);
    // trimA and trimH check if the king is on
    // file A or H respectively
    bitboard_t trimA, trimH;
    trimA = b & clearFile[0];
    trimH = b & clearFile[7];
    aux_moves[0] = trimA >> 9;
    aux_moves[1] = b >> 8;
    aux_moves[2] = trimH >> 7;
    aux_moves[3] = trimA >> 1;
    aux_moves[4] = trimH << 1;
    aux_moves[5] = trimA << 7;
    aux_moves[6] = b << 8;
    aux_moves[7] = trimH << 9;
    if (b & boards[5])
    {
        for (unsigned int i = 0; i < 8; i++)
            if (!(aux_moves[i] & allWhites))
                moves.push_back(aux_moves[i]);
    }
    else
    {
        for (unsigned int i = 0; i < 8; i++)
            if (!(aux_moves[i] & allBlacks))
                moves.push_back(aux_moves[i]);
    }
}

// return all VALID moves
bitboard_t ChessBoard::getKingAllMoves(bitboard_t b) {
    bitboard_t ret = 0;
    vector<bitboard_t> moves;
    getKingMoves(moves, b);
    for (unsigned int i = 0; i < moves.size(); i++) {
        ret |= moves[i];
    }
    return ret;
}

bitboard_t ChessBoard::getKingRandomMove(bitboard_t b) {
    bitboard_t ret = 0;
    vector<bitboard_t> moves;
    getKingMoves(moves, b);
    srand(time(NULL));
    if (moves.size())
        ret = moves[rand() % moves.size()];
    return ret;
}

void ChessBoard::getRooksMoves (vector <bitboard_t> &moves, bitboard_t b){
    pair<int, int> initialCoords = getCoords(b);
    bitboard_t currentPlayerPieces, opponentPieces;
    if (b & boards[1])
    {
        currentPlayerPieces = allWhites;
        opponentPieces = allBlacks;
    }
    else
    {
        currentPlayerPieces = allBlacks;
        opponentPieces = allWhites;
    }
    //return (maskFile[initialCoords.second-1] ^ maskRank[initialCoords.first-1]);
    int x = initialCoords.first - 1;
    int y = initialCoords.second - 1;
    x--;
    int i = 1;
    while (x >= 0)
    {
        bitboard_t tmp = b >> (8 * i);
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            x--;
        } 
        else
            break;
        if (tmp & opponentPieces)
            break;
    }
    x = initialCoords.first+1;
    i = 1;
    while (x < 9)
    {
        bitboard_t tmp = b << (8 * i);
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            x++;
        } 
        else
            break;
        if (tmp & opponentPieces)
            break;
    }
    i = 1;
    while (y > 0)
    {
        bitboard_t tmp = b >> i;
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            y--;
        } 
        else
            break;
        if (tmp & opponentPieces)
            break;
    }
    y = initialCoords.second+1;
    i = 1;
    while (y < 9)
    {
        bitboard_t tmp = b << i;
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            y++;
        } 
        else
            break;
        if (tmp & opponentPieces)
            break;
    }
}

// return all VALID moves
bitboard_t ChessBoard::getRooksAllMoves (bitboard_t b){
    bitboard_t ret = 0;
    vector<bitboard_t> moves;
    getRooksMoves(moves, b);
    for (unsigned int i = 0; i < moves.size(); i++) {
        ret |= moves[i];
    }
    return ret;
}

bitboard_t ChessBoard::getRooksRandomMove (bitboard_t b)
{
    bitboard_t ret = 0;
    vector<bitboard_t> moves;
    getRooksMoves(moves, b);
    srand(time(NULL));
    if (moves.size())
        ret = moves[rand() % moves.size()];
    return ret;
}

/*
 * - 1 - 2 -
 * 0 - - - 3
 * - - N - -
 * 7 - - - 4
 * - 6 - 5 -
 */
void ChessBoard::getKnightMoves(vector<bitboard_t>& moves, bitboard_t b) {
    vector<bitboard_t> aux_moves(8, 0);
    // trimAB and trimGH check if the knight is on
    // files (A, B) or (G, H) respectively
    bitboard_t trimAB, trimGH, trimA, trimH;
    trimAB = b & clearFile[0] & clearFile[1];
    trimGH = b & clearFile[7] & clearFile[6];
    trimA = b & clearFile[0];
    trimH = b & clearFile[7];
    aux_moves[0] = trimAB >> 10;
    aux_moves[1] = trimA >> 17;
    aux_moves[2] = trimH >> 15;
    aux_moves[3] = trimGH >> 6;
    aux_moves[4] = trimGH << 10;
    aux_moves[5] = trimH << 17;
    aux_moves[6] = trimA << 15;
    aux_moves[7] = trimAB << 6;
    if (b & boards[2])
    {
        for (int i = 0; i < 8; i++)
            if (!(aux_moves[i] & allWhites))
                moves.push_back(aux_moves[i]);
    }
    else
    {
        for (int i = 0; i < 8; i++)
            if (!(aux_moves[i] & allBlacks))
                moves.push_back(aux_moves[i]);
    }
}

// return all VALID moves
bitboard_t ChessBoard::getKnightAllMoves(bitboard_t b) {
    bitboard_t ret = 0;
    vector<bitboard_t> moves;
    getKnightMoves(moves, b);
    for (unsigned int i = 0; i < moves.size(); i++) {
        ret |= moves[i];
    }
    return ret;
}

bitboard_t ChessBoard::getKnightRandomMove(bitboard_t b) {
    bitboard_t ret = 0;
    vector<bitboard_t> moves;
    getKnightMoves(moves, b);
    srand(time(NULL));
    if (moves.size())
        ret = moves[rand() % moves.size()];
    return ret;
}

void ChessBoard::getBishopMoves(vector<bitboard_t> &moves, bitboard_t b)
{
    pair<int, int> initialCoords = getCoords(b);
    bitboard_t currentPlayerPieces, opponentPieces;
    if (b & boards[3])
    {
        currentPlayerPieces = allWhites;
        opponentPieces = allBlacks;
    }
    else
    {
        currentPlayerPieces = allBlacks;
        opponentPieces = allWhites;
    }

	//top,left
	int x = initialCoords.first;
	int y = initialCoords.second;
	int i = 1;
	while(x > 1 && y > 1)
	{
        bitboard_t tmp = b >> (i * 9);
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            x--;
            y--;
        }
        else
            break;
        if (tmp & opponentPieces)
            break;
	}
	//top,right
	x = initialCoords.first;
	y = initialCoords.second;
	i = 1;
	while(x > 1 && y < 8)
	{
        bitboard_t tmp = b >> (i * 7);
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            x--;
            y++;
        }
        else
            break;
        if (tmp & opponentPieces)
            break;
	}
	//bot,left
	x = initialCoords.first;
	y = initialCoords.second;
	i = 1;
	while(x < 8 && y > 1)
	{
        bitboard_t tmp = b << (i * 7);
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            x++;
            y--;
        }
        else
            break;
        if (tmp & opponentPieces)
            break;
	}
	//bot,right
	x = initialCoords.first;
	y = initialCoords.second;
	i = 1;
	while(x < 8 && y < 8)
	{
        bitboard_t tmp = b << (i * 9);
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            x++;
            y++;
        }
        else
            break;
        if (tmp & opponentPieces)
            break;
	}
}

// return all VALID moves
bitboard_t ChessBoard::getBishopAllMoves(bitboard_t b)
{
    bitboard_t ret = 0;
    vector<bitboard_t> moves;
    getBishopMoves(moves, b);
    for (unsigned int i = 0; i < moves.size(); i++) {
        ret |= moves[i];
    }
    return ret;
}

bitboard_t ChessBoard::getBishopRandomMove (bitboard_t b)
{
    bitboard_t ret = 0;
    vector<bitboard_t> moves;
    getBishopMoves(moves, b);
    srand(time(NULL));
    if (moves.size())
        ret = moves[rand() % moves.size()];
    return ret;
}

void ChessBoard::getQueenMoves(vector<bitboard_t> &moves, bitboard_t b)
{
    pair<int, int> initialCoords = getCoords(b);
    bitboard_t currentPlayerPieces, opponentPieces;
    if (b & boards[4])
    {
        currentPlayerPieces = allWhites;
        opponentPieces = allBlacks;
    }
    else
    {
        currentPlayerPieces = allBlacks;
        opponentPieces = allWhites;
    }

	//top,left
	int x = initialCoords.first;
	int y = initialCoords.second;
	int i = 1;
	while(x > 1 && y > 1)
	{
        bitboard_t tmp = b >> (i * 9);
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            x--;
            y--;
        } 
        else 
            break;
        if (tmp & opponentPieces)
            break;
	}
	//top,right
	x = initialCoords.first;
	y = initialCoords.second;
	i = 1;
	while(x > 1 && y < 8)
	{
        bitboard_t tmp = b >> (i * 7);
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            x--;
            y++;
        } 
        else
            break;
        if (tmp & opponentPieces)
            break;
	}
	//bot,left
	x = initialCoords.first;
	y = initialCoords.second;
	i = 1;
	while(x < 8 && y > 1)
	{
        bitboard_t tmp = b << (i * 7);
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            x++;
            y--;
        } 
        else 
            break;
        if (tmp & opponentPieces)
            break;
	}
	//bot,right
	x = initialCoords.first;
	y = initialCoords.second;
	i = 1;
	while(x < 8 && y < 8)
	{
        bitboard_t tmp = b << (i * 9);
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            x++;
            y++;
        } 
        else 
            break;
        if (tmp & opponentPieces)
            break;
	}

    x = initialCoords.first - 1;
    y = initialCoords.second - 1;
    x--;
    i = 1;
    while (x >= 0)
    {
        bitboard_t tmp = b >> (8 * i);
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            x--;
        } 
        else
            break;
        if (tmp & opponentPieces)
            break;
    }
    x = initialCoords.first+1;
    i = 1;
    while (x < 9)
    {
        bitboard_t tmp = b << (8 * i);
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            x++;
        } 
        else
            break;
        if (tmp & opponentPieces)
            break;
    }
    i = 1;
    while (y > 0)
    {
        bitboard_t tmp = b >> i;
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            y--;
        } 
        else
            break;
        if (tmp & opponentPieces)
            break;
    }
    y = initialCoords.second+1;
    i = 1;
    while (y < 9)
    {
        bitboard_t tmp = b << i;
        if (!(tmp & currentPlayerPieces)) {
            moves.push_back(tmp);
            i++;
            y++;
        } 
        else
            break;
        if (tmp & opponentPieces)
            break;
    }
}

// return all VALID moves
bitboard_t ChessBoard::getQueenAllMoves(bitboard_t b)
{
    bitboard_t ret = 0;
    vector<bitboard_t> moves;
    getQueenMoves(moves, b);
    cout << "pizda" << moves.size() << endl;
    for (unsigned int i = 0; i < moves.size(); i++) {
        ret |= moves[i];
    }
    return ret;
}

bitboard_t ChessBoard::getQueenRandomMove (bitboard_t b)
{
    bitboard_t ret = 0;
    vector<bitboard_t> moves;
    getQueenMoves(moves, b);
    srand(time(NULL));
    if (moves.size())
        ret = moves[rand() % moves.size()];
    return ret;

}

/*
 * - - -
 * - P -
 * - 0 -
 * - 1 -
 */
void ChessBoard::getWhitePawnMoves(vector<bitboard_t>& moves, bitboard_t b) {
    pair<int, int> initial_coords = getCoords(b);
    if (!((b << 8) & allWhites) && !((b << 8) & allBlacks))
    {
        moves.push_back(b << 8);
        if ((initial_coords.first == 2) && !((b << 16) & allWhites) && !((b << 16) & allBlacks))
            moves.push_back(b << 16);
    }
    if (allBlacks & (b << 7))
        moves.push_back(b << 7);
    if (allBlacks & (b << 9))
        moves.push_back(b << 9);
}
 
// return all VALID moves
bitboard_t ChessBoard::getWhitePawnAllMoves(bitboard_t b) {
    bitboard_t ret = 0;
    vector<bitboard_t> moves;
    getWhitePawnMoves(moves, b);
    if ((moves.size() > 1) && (moves[0] & allWhites) && ((moves[0] << 8) & moves[1]))
    {
        for (unsigned int i = 2; i < moves.size(); i++)
            ret |= moves[i];
        return ret;
    }
    for (unsigned int i = 0; i < moves.size(); i++)
        ret |= moves[i];
    ret &= ~allWhites;
    return ret;
}
 
bitboard_t ChessBoard::getWhitePawnRandomMove(bitboard_t b) {
    bitboard_t ret = 0;
    vector<bitboard_t> moves;
    getWhitePawnMoves(moves, b);
    srand(time(NULL));
    if (moves.size())
        ret = moves[rand() % moves.size()];
    return ret;
}
 
/*
 * - 1 -
 * - 0 -
 * - P -
 * - - -
 */
void ChessBoard::getBlackPawnMoves(vector<bitboard_t>& moves, bitboard_t b) {
    pair<int, int> initial_coords = getCoords(b);
    if (!((b >> 8) & allWhites) && !((b >> 8) & allBlacks))
    {
        moves.push_back(b >> 8);
        if ((initial_coords.first == 7) && !((b >> 16) & allWhites) && !((b >> 16) & allBlacks))
            moves.push_back(b >> 16);
    }
    if (allBlacks & (b >> 7))
        moves.push_back(b >> 7);
    if (allBlacks & (b >> 9))
        moves.push_back(b >> 9);
}
 
// return all VALID moves
bitboard_t ChessBoard::getBlackPawnAllMoves(bitboard_t b) {
    bitboard_t ret = 0ULL;
    std::vector<bitboard_t> moves;
    getBlackPawnMoves(moves, b);

    if ((moves.size() > 1) && (moves[0] & allBlacks) && ((moves[0] << 8) & moves[1]))
    {
        for (unsigned int i = 2; i < moves.size(); i++)
            ret |= moves[i];
        return ret;
    }
    for (unsigned int i = 0; i < moves.size(); i++)
        ret |= moves[i];
    ret &= ~allBlacks;
    return ret;
}
 
bitboard_t ChessBoard::getBlackPawnRandomMove(bitboard_t b) {
    vector<bitboard_t> moves;
    getBlackPawnMoves(moves, b);
    srand(time(NULL));
    return moves[rand() % moves.size()];
}

//checks if a kingside castling is possible
bool ChessBoard::canKingsideCastling()
{
	bitboard_t king;
	vector<pair<bitboard_t, int> > attackers;
	//check if king or king's rook have moved
	if(current_player == WHITE)
	{
		if(whiteKingMoved || whiteRookKingMoved)
			return false;
		king = boards[5];
	}
	else
	{
		if(blackKingMoved || blackRookKingMoved)
			return false;
		king = boards[11];
	}
	//check if check from, through or to
	isCheck(attackers);
	if(attackers.size() > 0)
		return false;
	if((king<<1 & allPieces) || (king<<2 & allPieces))
		return false;

	attackers.clear();
	isCheck(attackers, king<<1);
	int s1 = attackers.size();
	attackers.clear();
	isCheck(attackers, king<<2);
	int s2 = attackers.size();
	if(s1 > 0 || s2 > 0)
		return false;
	return true;
}

//checks if a queenside castling is possible
bool ChessBoard::canQueensideCastling()
{
	bitboard_t king;
	vector<pair<bitboard_t, int> > attackers;
	//check if king or king's rook have moved
	if(current_player == WHITE)
	{
		if(whiteKingMoved || whiteRookQueenMoved)
			return false;
		king = boards[5];
	}
	else
	{
		if(blackKingMoved || blackRookQueenMoved)
			return false;
		king = boards[11];
	}
	//check if check from, through or to
	isCheck(attackers);
	if(attackers.size() > 0)
	{
		return false;
	}

	if((king>>1 & allPieces) || (king>>2 & allPieces) || (king>>3 & allPieces))
	{
		return false;
	}

	attackers.clear();
	isCheck(attackers, king>>1);
	int s1 = attackers.size();

	attackers.clear();
	isCheck(attackers, king>>2);
	int s2 = attackers.size();

	if(s1 > 0 || s2 > 0)
	{
		return false;
	}
	return true;
}

bool ChessBoard::doKingsideCastling()
{
	if(!canKingsideCastling())
	{
		//this should be checked before calling
		//doCastling, so this if should be commented
		return false;
	}
	if(current_player == WHITE)
	{
		//erasing king and rook from allPieces
		allPieces &= ~boards[5];
		//we know that the king's rook has not moved
		allPieces &= ~square[7];
		//same for allWhites
		allWhites &= ~boards[5];
		allWhites &= ~square[7];
		
		boards[1] &= ~square[7];
		boards[1] |= square[5];
		boards[5] = boards[5]<<2;

		allPieces |= boards[1] | boards[5];
		allWhites |= boards[1] | boards[5];
		
		whiteKingMoved = true;
		whiteRookKingMoved = true;

		return true;
	}
	else
	{
		//erasing king and rook from allPieces
		allPieces &= ~boards[11];
		//we know that the king rook has not moved
		allPieces &= ~square[63];
		//same for allWhites
		allBlacks &= ~boards[11];
		allBlacks &= ~square[63];
		
		boards[7] &= ~square[63];
		boards[7] |= square[61];
		boards[11] = boards[11]<<2;

		allPieces |= boards[11] | boards[7];
		allBlacks |= boards[11] | boards[7];
		
		blackKingMoved = true;
		blackRookKingMoved = true;

		return true;
	}
	return false;
}

bool ChessBoard::doQueensideCastling()
{
	if(!canQueensideCastling())
	{
		//this should be checked before calling
		//doCastling, so this if should be commented
		return false;
	}
	if(current_player == WHITE)
	{
		//erasing king and rook from allPieces
		allPieces &= ~boards[5];
		//we know that the king's rook has not moved
		allPieces &= ~square[0];
		//same for allWhites
		allWhites &= ~boards[5];
		allWhites &= ~square[0];
		
		boards[1] &= ~square[0];
		boards[1] |= square[3];
		boards[5] = boards[5]>>2;

		allPieces |= boards[1] | boards[5];
		allWhites |= boards[1] | boards[5];
	
		whiteKingMoved = true;
		whiteRookQueenMoved = true;

		return true;
	}
	else
	{
		//erasing king and rook from allPieces
		allPieces &= ~boards[11];
		//we know that the king rook has not moved
		allPieces &= ~square[56];
		//same for allWhites
		allBlacks &= ~boards[11];
		allBlacks &= ~square[56];
		
		boards[7] &= ~square[56];
		boards[7] |= square[59];
		boards[11] = boards[11]>>2;

		allPieces |= boards[11] | boards[7];
		allBlacks |= boards[11] | boards[7];
		
		blackKingMoved = true;
		blackRookQueenMoved = true;

		return true;
	}
	return false;
}

void ChessBoard::getAllMoves(vector<pair<bitboard_t, bitboard_t> > &all_moves) {
    vector<bitboard_t> pieces, moves;
    all_moves.clear();
    int inc;

    if (current_player == WHITE) {
        inc = 0;
    }
    else {
        inc = 6;
    }

    // pawns
    pieces = split(boards[inc]);
    for (unsigned int i = 0; i < pieces.size(); i++) {
        moves.clear();
        if (current_player == WHITE)
            getWhitePawnMoves(moves, pieces[i]);
        else
            getBlackPawnMoves(moves, pieces[i]);

        for (unsigned int j = 0; j < moves.size(); j++) {
            all_moves.push_back(make_pair(pieces[i], moves[j]));
        }
    }
    inc++;

    // rooks
    pieces = split(boards[inc]);
    for (unsigned int i = 0; i < pieces.size(); i++) {
        moves.clear();
        getRooksMoves(moves, pieces[i]);
        for (unsigned int j = 0; j < moves.size(); j++) {
            all_moves.push_back(make_pair(pieces[i], moves[j]));
        }
    }
    inc++;

    // kinghts
    pieces = split(boards[inc]);
    for (unsigned int i = 0; i < pieces.size(); i++) {
        moves.clear();
        getKnightMoves(moves, pieces[i]);
        for (unsigned int j = 0; j < moves.size(); j++) {
            all_moves.push_back(make_pair(pieces[i], moves[j]));
        }
    }
    inc++;

    // bishops
    pieces = split(boards[inc]);
    for (unsigned int i = 0; i < pieces.size(); i++) {
        moves.clear();
        getBishopMoves(moves, pieces[i]);
        for (unsigned int j = 0; j < moves.size(); j++) {
            all_moves.push_back(make_pair(pieces[i], moves[j]));
        }
    }
    inc++;

    // queen
    moves.clear();
    getQueenMoves(moves, boards[inc]);
    for (unsigned int j = 0; j < moves.size(); j++) {
        all_moves.push_back(make_pair(boards[inc], moves[j]));
    }
    inc++;

    // king 
    moves.clear();
    getKingMoves(moves, boards[inc]);
    for (unsigned int j = 0; j < moves.size(); j++) {
        all_moves.push_back(make_pair(boards[inc], moves[j]));
    }
}
