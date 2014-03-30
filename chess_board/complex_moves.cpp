#include "chess_board.h"

using namespace std;

bitboard_t ChessBoard::getOutOfCheck() {
    vector<pair<bitboard_t, int> > attackers;
    isCheck(attackers);
    vector<bitboard_t> pieces, all_pieces;

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
    vector<bitboard_t> king_moves;
    if (current_player == WHITE) {
        getKingMoves(king_moves, boards[5]);
    }
    else {
        getKingMoves(king_moves, boards[11]);
    }
    return 0ULL;
}
