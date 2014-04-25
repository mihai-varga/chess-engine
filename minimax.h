#include "chess_board/chess_board.h"

using namespace std;
/**
 * Implementarea de negamax cu alpha-beta pruning
 * Intoarce o pereche <x, y> unde x este cel mai bun scor
 * care poate fi obtinut de jucatorul aflat la mutare,
 * iar y este mutarea propriu-zisa
 */
pair<int, PairBB > minimax_abeta(ChessBoard cb, player_t player, int depth, int alfa, int beta)
{
    PairBB bestMove(0ULL, 0ULL);

	if(depth == 0)
	{
		//std::pair<int,Move> aux(cb.eval(player), bestMove);
		//return aux;
        return make_pair(cb.evaluate(player), bestMove);
	}

	int score;
    
    vector<PairBB> moves;
    cb.getAllMoves(moves);
    ChessBoard newCb;
	
	for(unsigned int i = 0; i < moves.size(); i++)
	{
        newCb = cb;
        newCb.setMove(moves[i].first, moves[i].second);
		
        pair<int, PairBB> aux = minimax_abeta(newCb, player, 
                depth - 1, -beta, -alfa);
		score = -1 * aux.first;
		if(score >= beta)
        {
			return make_pair(beta, moves[i]);
        }
		if(score > alfa)
		{
			alfa = score;
			bestMove = moves[i];
		}
	}

    //return std::pair<int, Move>(alfa, bestMove);
    return make_pair(alfa, bestMove);
}
