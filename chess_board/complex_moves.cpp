#include "chess_board.h"

using namespace std;

bitboard_t getPath(bitboard_t from, bitboard_t to, int type) {
    int from_pos = 0, to_pos = 0;
    int inc, diff;
    while (from_pos < 64 && (from != 1ULL << from_pos)) {
        from_pos++;
    }
    while (to_pos < 64 && (to != 1ULL << to_pos)) {
        to_pos++;
    }

    diff = to_pos - from_pos;
    bitboard_t path = 0ULL;

    switch (type) {
        // diagonal: bishp & queen
        case 0: {
            if ((diff > 0 && diff % 9 != 0)
                    || (diff < 0 && diff % 7 != 0)) {
                return path;
            }
            if (from_pos % 8 <= 4)
                if (from_pos < to_pos)
                    inc = 9;
                else
                    inc = -7;
            else
                if (from_pos < to_pos)
                    inc = 7;
                else
                    inc = -9;
            }
            break;

        // vertical: pawn, rook, queen 
        case 1: {
            if (diff % 8 != 0)
                return path;
            if (diff > 0)
                inc = 8;
            else
                inc = -8;
            }
            break;

        // horizontal: rook, queen
        case 2: {
            if (diff / 8 != 0)
                return path;
            if (diff > 0)
                inc = 1;
            else
                inc = -1;
            }
            break;
        default: return path;
    }

    while (from_pos + inc != to_pos) {
        from_pos += inc;
        path |= 1ULL << from_pos;
    }
    return path;
}

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
