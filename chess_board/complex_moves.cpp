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

pair<bitboard_t, bitboard_t> ChessBoard::getOutOfCheck(vector<pair<bitboard_t, int> > &attackers) {
    vector<bitboard_t> pieces, all_pieces, moves;
    vector<pair<bitboard_t, bitboard_t> > all_moves; // from -> to
    bitboard_t king_pos;

    if (attackers.size() == 1) {
        // maybe we can capture the attacking piece
        if (current_player == WHITE) {
            king_pos = boards[5];
        }
        else {
            king_pos = boards[11];
        }

        getAllMoves(all_moves);

        // try to capture with a piece
        for (unsigned int i = 0; i < all_moves.size(); i++) {
            if (all_moves[i].second == attackers[0].first &&
                    isValid(all_moves[i].first, all_moves[i].second)) {
                return all_moves[i];
            }
        }

        //try to move a piece between the nazgul and its prey
        bitboard_t path = 0ULL;
        switch (attackers[0].second) {
            case 0: // white pawn
            case 6: // black pawn
                path = getPath(attackers[0].first, king_pos, 1);
                break;
            case 1: // white rook
            case 7: // black rook
                path |= getPath(attackers[0].first, king_pos, 1);
                path |= getPath(attackers[0].first, king_pos, 2);
                break;
            case 3: // white bishop
            case 9: // black bishop
                path = getPath(attackers[0].first, king_pos, 0);
                break;
            case 4: // white queen
            case 10: // black queen
                path |= getPath(attackers[0].first, king_pos, 0);
                path |= getPath(attackers[0].first, king_pos, 1);
                path |= getPath(attackers[0].first, king_pos, 2);
                break;
        }

        vector<bitboard_t> attack_blocking = split(path);
        for (unsigned int i = 0; i < all_moves.size(); i++) {
            for (unsigned int j = 0; j < attack_blocking.size(); j++) {
                if (all_moves[i].second == attack_blocking[i] && // block 
                        all_moves[i].first != king_pos && // not the king 
                        isValid(all_moves[i].first, all_moves[i].second)) { // no more check
                    return all_moves[i];
                }
            }
        }
    }

    // There are more than 1 attackers or we couldn't caputer/block the attacker
    // so we try to move the king
    vector<bitboard_t> king_moves;
    getKingMoves(king_moves, king_pos);
    for (unsigned int i = 0; i > king_moves.size(); i++) {
        if (isValid(king_pos, king_moves[i])) {
            return make_pair(king_pos, king_moves[i]);
        }
    }
    // check mate
    return make_pair(0ULL, 0ULL);
}
