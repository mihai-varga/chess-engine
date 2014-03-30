#include "chess_board.h"
#include <iostream>
#include <vector>
#include <climits>
#include <cstdlib>
#include <time.h>

#include <stdio.h>

using namespace std;

void ChessBoard::setMove(ChessBoard::bitboard_t from, ChessBoard::bitboard_t to) {
    int index = ChessBoard::getBoard(from);
    ChessBoard::boards[index] = (ChessBoard::boards[index] & ~from) | to;
    ChessBoard::allPieces = (ChessBoard::allPieces & ~from) | to;
    // if white is moving
    if (from & ChessBoard::allWhites) {
        if (index == 0) { // if moving a pawn 
            if (to & maskRank[7]) { // if the pawn has reached the last rank
                ChessBoard::boards[index] = ChessBoard::boards[index] & ~to; // delete the pawn
                ChessBoard::boards[4] = ChessBoard::boards[4] | to; // add the queen
            }
        }
        ChessBoard::allWhites = (ChessBoard::allWhites & ~from) | to;
        if (to & ChessBoard::allBlacks) { // if white attacks black
            int index_black = ChessBoard::getBoard(to);
            ChessBoard::boards[index_black] = ChessBoard::boards[index_black] & ~to;
            ChessBoard::allBlacks = ChessBoard::allBlacks & ~to;
        }
    } else { // if black is moving
        if (index == 6) { // if moving a pawn 
            if (to & maskRank[0]) { // if the pawn has reached the last rank
                ChessBoard::boards[index] = ChessBoard::boards[index] & ~to; // delete the pawn
                ChessBoard::boards[10] = ChessBoard::boards[10] | to; // add the queen
            }
        }
        ChessBoard::allBlacks = (ChessBoard::allBlacks & ~from) | to;
        if (to & ChessBoard::allWhites) { // if black attacks white
            int index_white = ChessBoard::getBoard(to);
            ChessBoard::boards[index_white] = ChessBoard::boards[index_white] & ~to;
            ChessBoard::allWhites = ChessBoard::allWhites & ~to;
        }
    }
}

pair<ChessBoard::bitboard_t, ChessBoard::bitboard_t> ChessBoard::getNextMove()
{
    ChessBoard::bitboard_t from, to;
    int start, chessman;
    bool find_move = false;
    srand(time(NULL));
    if (ChessBoard::current_player == WHITE)
        start = 0;
    else
        start = 6;
    while (!find_move)
    {
        //do
            chessman = rand()%6;
        //while(chessman == 1);
        vector<ChessBoard::bitboard_t> aux_vect;
        switch(chessman)
        {
            case 0: aux_vect = split(ChessBoard::boards[start]);
                    if (aux_vect.size())
                    {
                        from = aux_vect[rand() % aux_vect.size()];
                        if (start == 6)
                            to = getBlackPawnRandomMove(from);
                        else
                            to = getWhitePawnRandomMove(from);
                        if (to)
                        {
                            pair<ChessBoard::bitboard_t, ChessBoard::bitboard_t> move(from, to);
                            find_move = true;
                            return move;
                        }
                    }
                    break;
            case 1: aux_vect = split(ChessBoard::boards[start+1]);
                    if (aux_vect.size())
                    {
                        from = aux_vect[rand() % aux_vect.size()];
                        to = getRooksRandomMove(from);
                        if (to)
                        {
                            pair<ChessBoard::bitboard_t, ChessBoard::bitboard_t> move(from, to);
                            find_move = true;
                            return move;
                        }
                    }
                    break;
            case 2: aux_vect = split(ChessBoard::boards[start + 2]);
                    if (aux_vect.size())
                    {
                        from = aux_vect[rand() % aux_vect.size()];
                        to = getKnightRandomMove(from);
                        if (to)
                        {
                            pair<ChessBoard::bitboard_t, ChessBoard::bitboard_t> move(from, to);
                            find_move = true;
                            return move;
                        }
                    }
                    break;
            case 3: aux_vect = split(ChessBoard::boards[start + 3]);
                    if (aux_vect.size())
                    {
                        from = aux_vect[rand() % aux_vect.size()];
                        to = getBishopRandomMove(from);
                        if (to)
                        {
                            pair<ChessBoard::bitboard_t, ChessBoard::bitboard_t> move(from, to);
                            find_move = true;
                            return move;
                        }
                    }
                    break;
            case 4: aux_vect = split(ChessBoard::boards[start + 4]);
                    if (aux_vect.size())
                    {
                        from = aux_vect[rand() % aux_vect.size()];
                        to = getQueenRandomMove(from);
                        if (to)
                        {
                            pair<ChessBoard::bitboard_t, ChessBoard::bitboard_t> move(from, to);
                            find_move = true;
                            return move;
                        }
                    }
                    break;
            case 5: aux_vect = split(ChessBoard::boards[start + 5]);
                    if (aux_vect.size())
                    {
                        from = aux_vect[rand() % aux_vect.size()];
                        to = getKingRandomMove(from);
                        if (to)
                        {
                            pair<ChessBoard::bitboard_t, ChessBoard::bitboard_t> move(from, to);
                            find_move = true;
                            return move;
                        }
                    }
                    break;
        }
    }
    pair<ChessBoard::bitboard_t, ChessBoard::bitboard_t> move(from, to);
    return move;
}

bool ChessBoard::isValid(ChessBoard::bitboard_t from, ChessBoard::bitboard_t to) {
    std::vector<std::pair<ChessBoard::bitboard_t, int> > attackers;
    ChessBoard::printBoard(from);
    ChessBoard::printBoard(to);
    if (from == 0 || to == 0) {
        return false;
    }
    // if WHITE moves
    if (current_player == WHITE) {
        // check if the king will be in check
        if (from & ChessBoard::boards[5]) {
            isCheck(attackers, to);
            if (attackers.size() != 0)
                return false;
        }
        if (from & ChessBoard::allWhites) {
            if (to & ChessBoard::allWhites) {
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
        if (from & ChessBoard::boards[11]) {
            isCheck(attackers, to);
            if (attackers.size() != 0)
                return false;
        }
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


/*
 * checks if current's player king is in check
 *
 * ATTENTION !!!!
 * if king != 0 it checks from that position assuming it is the current player's king
 * TODO implement to check if king is not the current player's king
 * TODO implement isCheck using rays
 */
void ChessBoard::isCheck(vector< pair<ChessBoard::bitboard_t, int> >& attackers,
                                 ChessBoard::bitboard_t king) {
    //ChessBoard::bitboard_t opponentAllMoves = 0ULL;
    ChessBoard::player_t opponentColor = current_player == WHITE ? BLACK : WHITE;
    vector<ChessBoard::bitboard_t> aux;
    ChessBoard::bitboard_t allOpponentMoves = 0ULL;
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
        aux = split(boards[4]);
        for (unsigned int i = 0; i < aux.size(); i++) {
            if (king & getQueenAllMoves(aux[i])) {
                attackers.push_back(make_pair(aux[i], 4));
            }
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
        printBoard(allOpponentMoves);
    } else {
        // check queen
        if (king & getQueenAllMoves(boards[10])) {
            ChessBoard::bitboard_t queen_pos = split(boards[10])[0];
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

void ChessBoard::isCheck(vector< pair<ChessBoard::bitboard_t, int> >& attackers) {
	this->isCheck(attackers, 0ULL);
}

bool ChessBoard::isCheckMate() {

    return false;
}

ChessBoard::bitboard_t ChessBoard::getOutOfCheck() {
    vector<pair<ChessBoard::bitboard_t, int> > attackers;
    isCheck(attackers);
    vector<ChessBoard::bitboard_t> pieces, all_pieces;

    if (attackers.size() == 1) {
        // maybe we can capture the attacking piece
        if (current_player == WHITE) {
            // get all white pieces togheter
            for (int i = 0; i <= 5; i++) {
                pieces = split(boards[i]);
                all_pieces.insert(all_pieces.begin(), pieces.begin(), pieces.end());
            }
        }
        else {
            // get all black pieces togheter
            for (int i = 0; i <= 5; i++) {
                pieces = split(boards[i]);
                all_pieces.insert(all_pieces.begin(), pieces.begin(), pieces.end());
            }
        }

        // try to capture with a piece
        for (unsigned int i = 0; i < all_pieces.size(); i++) {
            if (isValid(pieces[i], attackers[0].first)) {
                return pieces[i];
            }
        }
    }
    // we try to move the king
    vector<ChessBoard::bitboard_t> king_moves;
    if (current_player == WHITE) {
        getKingMoves(king_moves, boards[5]);
    }
    else {
        getKingMoves(king_moves, boards[11]);
    }
    return 0ULL;
}



/*
 * 0 1 2
 * 3 K 4
 * 5 6 7
 */
void ChessBoard::getKingMoves(vector<ChessBoard::bitboard_t>& moves, ChessBoard::bitboard_t b) {
    vector<ChessBoard::bitboard_t> aux_moves(8, 0);
    // trimA and trimH check if the king is on
    // file A or H respectively
    ChessBoard::bitboard_t trimA, trimH;
    trimA = b & ChessBoard::clearFile[0];
    trimH = b & ChessBoard::clearFile[7];
    aux_moves[0] = trimA >> 9;
    aux_moves[1] = b >> 8;
    aux_moves[2] = trimH >> 7;
    aux_moves[3] = trimA >> 1;
    aux_moves[4] = trimH << 1;
    aux_moves[5] = trimA << 7;
    aux_moves[6] = b << 8;
    aux_moves[7] = trimH << 9;
    if (b & ChessBoard::boards[5])
    {
        for (unsigned int i = 0; i < 8; i++)
            if (!(aux_moves[i] & ChessBoard::allWhites))
                moves.push_back(aux_moves[i]);
    }
    else
    {
        for (unsigned int i = 0; i < 8; i++)
            if (!(aux_moves[i] & ChessBoard::allBlacks))
                moves.push_back(aux_moves[i]);
    }
}

ChessBoard::bitboard_t ChessBoard::getKingAllMoves(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    vector<ChessBoard::bitboard_t> moves;
    getKingMoves(moves, b);
    for (unsigned int i = 0; i < moves.size(); i++) {
        ret |= moves[i];
    }
    return ret;
}

ChessBoard::bitboard_t ChessBoard::getKingRandomMove(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    vector<ChessBoard::bitboard_t> moves;
    getKingMoves(moves, b);
    srand(time(NULL));
    if (moves.size())
        ret = moves[rand() % moves.size()];
    return ret;
}

void ChessBoard::getRooksMoves (vector <ChessBoard::bitboard_t> &moves, ChessBoard::bitboard_t b){
    pair<int, int> initialCoords = ChessBoard::getCoords(b);
    bitboard_t currentPlayerPieces, opponentPieces;
    if (b & ChessBoard::boards[1])
    {
        currentPlayerPieces = allWhites;
        opponentPieces = allBlacks;
    }
    else
    {
        currentPlayerPieces = allBlacks;
        opponentPieces = allWhites;
    }
    //return (ChessBoard::maskFile[initialCoords.second-1] ^ ChessBoard::maskRank[initialCoords.first-1]);
    int x = initialCoords.first - 1;
    int y = initialCoords.second - 1;
    x--;
    int i = 1;
    while (x >= 0)
    {
        ChessBoard::bitboard_t tmp = b >> (8 * i);
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
        ChessBoard::bitboard_t tmp = b << (8 * i);
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
        ChessBoard::bitboard_t tmp = b >> i;
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
        ChessBoard::bitboard_t tmp = b << i;
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

ChessBoard::bitboard_t ChessBoard::getRooksAllMoves (ChessBoard::bitboard_t b){
    pair<int, int> initialCoords = ChessBoard::getCoords(b);
    ChessBoard::bitboard_t ret = 0;
    bitboard_t currentPlayerPieces, opponentPieces;
    if (b & ChessBoard::boards[1])
    {
        currentPlayerPieces = allWhites;
        opponentPieces = allBlacks;
    }
    else
    {
        currentPlayerPieces = allBlacks;
        opponentPieces = allWhites;
    }
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
        ChessBoard::bitboard_t tmp = b << (8 * i);
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
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
        ChessBoard::bitboard_t tmp = b >> i;
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
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
        ChessBoard::bitboard_t tmp = b << i;
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
            i++;
            y++;
        } 
        else
            break;
        if (tmp & opponentPieces)
            break;
    }
    return ret;
}

ChessBoard::bitboard_t ChessBoard::getRooksRandomMove (ChessBoard::bitboard_t b)
{
    ChessBoard::bitboard_t ret = 0;
    vector<ChessBoard::bitboard_t> moves;
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
void ChessBoard::getKnightMoves(vector<ChessBoard::bitboard_t>& moves, ChessBoard::bitboard_t b) {
    vector<ChessBoard::bitboard_t> aux_moves(8, 0);
    // trimAB and trimGH check if the knight is on
    // files (A, B) or (G, H) respectively
    ChessBoard::bitboard_t trimAB, trimGH, trimA, trimH;
    trimAB = b & ChessBoard::clearFile[0] & ChessBoard::clearFile[1];
    trimGH = b & ChessBoard::clearFile[7] & ChessBoard::clearFile[6];
    trimA = b & ChessBoard::clearFile[0];
    trimH = b & ChessBoard::clearFile[7];
    aux_moves[0] = trimAB >> 10;
    aux_moves[1] = trimA >> 17;
    aux_moves[2] = trimH >> 15;
    aux_moves[3] = trimGH >> 6;
    aux_moves[4] = trimGH << 10;
    aux_moves[5] = trimH << 17;
    aux_moves[6] = trimA << 15;
    aux_moves[7] = trimAB << 6;
    if (b & ChessBoard::boards[2])
    {
        for (int i = 0; i < 8; i++)
            if (!(aux_moves[i] & ChessBoard::allWhites))
                moves.push_back(aux_moves[i]);
    }
    else
    {
        for (int i = 0; i < 8; i++)
            if (!(aux_moves[i] & ChessBoard::allBlacks))
                moves.push_back(aux_moves[i]);
    }
}

ChessBoard::bitboard_t ChessBoard::getKnightAllMoves(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    vector<ChessBoard::bitboard_t> moves;
    getKnightMoves(moves, b);
    for (unsigned int i = 0; i < moves.size(); i++) {
        ret |= moves[i];
    }
    return ret;
}

ChessBoard::bitboard_t ChessBoard::getKnightRandomMove(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    vector<ChessBoard::bitboard_t> moves;
    getKnightMoves(moves, b);
    srand(time(NULL));
    if (moves.size())
        ret = moves[rand() % moves.size()];
    return ret;
}

void ChessBoard::getBishopMoves(vector<ChessBoard::bitboard_t> &moves, ChessBoard::bitboard_t b)
{
    pair<int, int> initialCoords = ChessBoard::getCoords(b);
    bitboard_t currentPlayerPieces, opponentPieces;
    if (b & ChessBoard::boards[3])
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
ChessBoard::bitboard_t ChessBoard::getBishopAllMoves(ChessBoard::bitboard_t b)
{
    pair<int, int> initialCoords = ChessBoard::getCoords(b);
	bitboard_t ret = 0;
    bitboard_t currentPlayerPieces, opponentPieces;
    if (b & ChessBoard::boards[3])
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
            ret |= tmp;
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
            ret |= tmp;
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
            ret |= tmp;
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
            ret |= tmp;
            i++;
            x++;
            y++;
        }
        else
            break;
        if (tmp & opponentPieces)
            break;
	}
	return ret;
}

ChessBoard::bitboard_t ChessBoard::getBishopRandomMove (ChessBoard::bitboard_t b)
{
    ChessBoard::bitboard_t ret = 0;
    vector<ChessBoard::bitboard_t> moves;
    getBishopMoves(moves, b);
    srand(time(NULL));
    if (moves.size())
        ret = moves[rand() % moves.size()];
    return ret;
}

void ChessBoard::getQueenMoves(vector<ChessBoard::bitboard_t> &moves, ChessBoard::bitboard_t b)
{
    pair<int, int> initialCoords = ChessBoard::getCoords(b);
    bitboard_t currentPlayerPieces, opponentPieces;
    if (b & ChessBoard::boards[4])
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
        ChessBoard::bitboard_t tmp = b >> (8 * i);
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
        ChessBoard::bitboard_t tmp = b << (8 * i);
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
        ChessBoard::bitboard_t tmp = b >> i;
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
        ChessBoard::bitboard_t tmp = b << i;
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

ChessBoard::bitboard_t ChessBoard::getQueenAllMoves(ChessBoard::bitboard_t b)
{
    pair<int, int> initialCoords = ChessBoard::getCoords(b);
	bitboard_t ret = 0;
    bitboard_t currentPlayerPieces, opponentPieces;
    if (b & ChessBoard::boards[4])
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
            ret |= tmp;
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
            ret |= tmp;
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
            ret |= tmp;
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
            ret |= tmp;
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
        ChessBoard::bitboard_t tmp = b >> (8 * i);
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
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
        ChessBoard::bitboard_t tmp = b << (8 * i);
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
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
        ChessBoard::bitboard_t tmp = b >> i;
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
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
        ChessBoard::bitboard_t tmp = b << i;
        if (!(tmp & currentPlayerPieces)) {
            ret |= tmp;
            i++;
            y++;
        } 
        else
            break;
        if (tmp & opponentPieces)
            break;
    }

	return ret;
}

ChessBoard::bitboard_t ChessBoard::getQueenRandomMove (ChessBoard::bitboard_t b)
{
    ChessBoard::bitboard_t ret = 0;
    vector<ChessBoard::bitboard_t> moves;
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
void ChessBoard::getWhitePawnMoves(vector<ChessBoard::bitboard_t>& moves, ChessBoard::bitboard_t b) {
    pair<int, int> initial_coords = ChessBoard::getCoords(b);
    if (!((b << 8) & ChessBoard::allWhites) && !((b << 8) & ChessBoard::allBlacks))
    {
        moves.push_back(b << 8);
        if ((initial_coords.first == 2) && !((b << 16) & ChessBoard::allWhites) && !((b << 16) & ChessBoard::allBlacks))
            moves.push_back(b << 16);
    }
    if (ChessBoard::allBlacks & (b << 7))
        moves.push_back(b << 7);
    if (ChessBoard::allBlacks & (b << 9))
        moves.push_back(b << 9);
}
 
ChessBoard::bitboard_t ChessBoard::getWhitePawnAllMoves(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    vector<ChessBoard::bitboard_t> moves;
    getWhitePawnMoves(moves, b);
    if ((moves.size() > 1) && (moves[0] & ChessBoard::allWhites) && ((moves[0] << 8) & moves[1]))
    {
        for (unsigned int i = 2; i < moves.size(); i++)
            ret |= moves[i];
        return ret;
    }
    for (unsigned int i = 0; i < moves.size(); i++)
        ret |= moves[i];
    ret &= ~ChessBoard::allWhites;
    return ret;
}
 
ChessBoard::bitboard_t ChessBoard::getWhitePawnRandomMove(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0;
    vector<ChessBoard::bitboard_t> moves;
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
void ChessBoard::getBlackPawnMoves(vector<ChessBoard::bitboard_t>& moves, ChessBoard::bitboard_t b) {
    pair<int, int> initial_coords = ChessBoard::getCoords(b);
    if (!((b >> 8) & ChessBoard::allWhites) && !((b >> 8) & ChessBoard::allBlacks))
    {
        moves.push_back(b >> 8);
        if ((initial_coords.first == 7) && !((b >> 16) & ChessBoard::allWhites) && !((b >> 16) & ChessBoard::allBlacks))
            moves.push_back(b >> 16);
    }
    if (ChessBoard::allBlacks & (b >> 7))
        moves.push_back(b >> 7);
    if (ChessBoard::allBlacks & (b >> 9))
        moves.push_back(b >> 9);
}
 
ChessBoard::bitboard_t ChessBoard::getBlackPawnAllMoves(ChessBoard::bitboard_t b) {
    ChessBoard::bitboard_t ret = 0ULL;
    std::vector<ChessBoard::bitboard_t> moves;
    getBlackPawnMoves(moves, b);

    if ((moves.size() > 1) && (moves[0] & ChessBoard::allBlacks) && ((moves[0] << 8) & moves[1]))
    {
        for (unsigned int i = 2; i < moves.size(); i++)
            ret |= moves[i];
        return ret;
    }
    for (unsigned int i = 0; i < moves.size(); i++)
        ret |= moves[i];
    ret &= ~ChessBoard::allBlacks;
    return ret;
}
 
ChessBoard::bitboard_t ChessBoard::getBlackPawnRandomMove(ChessBoard::bitboard_t b) {
    vector<ChessBoard::bitboard_t> moves;
    getBlackPawnMoves(moves, b);
    srand(time(NULL));
    return moves[rand() % moves.size()];
}

//checks if a kingside castling is possible
bool ChessBoard::canKingsideCastling()
{
	ChessBoard::bitboard_t king;
	vector<pair<ChessBoard::bitboard_t, int> > attackers;
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
	ChessBoard::bitboard_t king;
	vector<pair<ChessBoard::bitboard_t, int> > attackers;
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
