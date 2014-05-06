#include "chess_board/chess_board.h"
#include <iostream>

using namespace std;

/*
 * Implementarea de negamax cu alpha-beta pruning
 * Intoarce o pereche <int, PairBB> unde x este cel mai bun scor
 * care poate fi obtinut de jucatorul aflat la mutare,
 * iar y este mutarea propriu-zisa
 */
pair<int, PairBB> negamax_abeta(ChessBoard cb, player_t player, int depth, int alfa, int beta)
{
    PairBB bestMove(0ULL, 0ULL);

    if(depth == 0)
    {
        return make_pair(cb.evaluate(player), bestMove);
    }

    int score;
    vector<PairBB> moves;
    cb.getAllMoves(moves);
    ChessBoard newCb;
    if (moves.size() > 0)
        bestMove = moves[0];

    for(unsigned int i = 0; i < moves.size(); i++)
    {
        newCb = cb;
        newCb.setMove(moves[i].first, moves[i].second);
        newCb.move_index++;
        pair<int, PairBB> aux;

        if (player == WHITE)
        {
            newCb.current_player = BLACK;
            aux = negamax_abeta(newCb, BLACK, depth - 1, -beta, -alfa);
        }
        else
        {
            newCb.current_player = WHITE;
            aux = negamax_abeta(newCb, WHITE, depth - 1, -beta, -alfa);
        }

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

    return make_pair(alfa, bestMove);
}
