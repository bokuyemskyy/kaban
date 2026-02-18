#pragma once
#include "position.hpp"

class Evaluation {
   public:
    static constexpr int MATE_SCORE     = 30000;
    static constexpr int MATE_THRESHOLD = 29000;

    static int evaluate(const Position& position) {
        int mg_score[2] = {0, 0};
        int eg_score[2] = {0, 0};
        int game_phase  = 0;

        for (Square sq : Squares::all()) {
            Piece piece = position.at(sq);
            if (piece == Pieces::NONE) continue;

            PieceType type  = piece.type();
            Color     color = piece.color();
            int       c_idx = color.value();

            int pst_index = (color == Colors::WHITE) ? sq.value() : (sq.value() ^ 56);

            int mg_val = mg_value[type.value()];
            int eg_val = eg_value[type.value()];

            int mg_pst_val = 0;
            int eg_pst_val = 0;

            if (type == PieceTypes::KING) {
                mg_pst_val = king_mg_pst[pst_index];
                eg_pst_val = king_eg_pst[pst_index];
            } else {
                const int* table = getPstTable(type);
                if (table) {
                    mg_pst_val = table[pst_index];
                    eg_pst_val = table[pst_index];
                }
            }

            mg_score[c_idx] += (mg_val + mg_pst_val);
            eg_score[c_idx] += (eg_val + eg_pst_val);
            game_phase += game_phase_weights[type.value()];
        }

        if (game_phase > 24) game_phase = 24;
        int mg_phase = game_phase;
        int eg_phase = 24 - game_phase;

        int score_white =
            (mg_score[Colors::WHITE.value()] * mg_phase + eg_score[Colors::WHITE.value()] * eg_phase) / 24;
        int score_black =
            (mg_score[Colors::BLACK.value()] * mg_phase + eg_score[Colors::BLACK.value()] * eg_phase) / 24;

        return (position.us() == Colors::WHITE) ? (score_white - score_black) : (score_black - score_white);
    }

    static int pieceValue(PieceType pt) { return mg_value[pt.value()]; }

   private:
    // clang-format off
    inline static const int mg_value[6] = { 82, 337, 365, 477, 1025, 0 };
    inline static const int eg_value[6] = { 94, 281, 297, 512,  936, 0 };

    inline static const int game_phase_weights[6] = { 0, 1, 1, 2, 4, 0 };

    inline static const int pawn_pst[64] = {
          0,   0,   0,   0,   0,   0,   0,   0,
         98, 134,  61,  95,  68, 126,  34, -11,
         -6,   7,  26,  31,  65,  56,  25, -20,
        -14,  13,   6,  21,  23,  12,  17, -23,
        -27,  -2,  -5,  12,  17,   6,  10, -25,
        -26,  -4,  -4, -10,   3,   3,  33, -12,
        -35,  -1, -20, -23, -15,  24,  38, -22,
          0,   0,   0,   0,   0,   0,   0,   0
    };

    inline static const int knight_pst[64] = {
        -167, -89, -34, -49,  61, -97, -15, -107,
         -73, -41,  72,  36,  23,  62,   7,  -17,
         -47,  60,  37,  65,  84, 129,  73,   44,
          -9,  17,  19,  53,  37,  69,  18,   22,
         -13,   4,  16,  13,  28,  19,  21,   -8,
         -23,  -9,  12,  10,  19,  17,  25,  -16,
         -29, -53, -12,  -3,  -1,  18, -14,  -19,
        -105, -21, -58, -33, -17, -28, -19,  -23
    };

    inline static const int bishop_pst[64] = {
        -29,   4, -82, -37, -25, -42,   7,  -8,
        -26,  16, -18, -13,  30,  59,  18, -47,
        -16,  37,  43,  40,  35,  50,  37,  -2,
         -4,   5,  19,  50,  37,  37,   7,  -2,
         -6,  13,  13,  26,  34,  12,  10,   4,
          0,  15,  15,  15,  14,  27,  18,  10,
          4,  15,  16,   0,   7,  21,  33,   1,
        -33,  -3, -14, -21, -13, -12, -39, -21
    };

    inline static const int rook_pst[64] = {
         32,  42,  32,  51, 63,  9,  31,  43,
         27,  32,  58,  62, 80, 67,  26,  44,
         -5,  19,  26,  36, 17, 45,  61,  16,
        -24, -11,   7,  26, 24, 35,  -8, -20,
        -36, -26, -12,  -1,  9, -7,   6, -23,
        -45, -25, -16, -17,  3,  0,  -5, -33,
        -44, -16, -20,  -9, -1, 11,  -6, -71,
        -19, -13,   1,  17, 16,  7, -37, -26
    };

    inline static const int queen_pst[64] = {
        -28,   0,  29,  12,  59,  44,  43,  45,
        -24, -39,  -5,   1, -16,  57,  28,  54,
        -13, -17,   7,   8,  29,  56,  47,  57,
        -27, -27, -16, -16,  -1,  17,  -2,   1,
         -9, -26,  -9, -10,  -2,  -4,   3,  -3,
        -14,   2, -11,  -2,  -5,   2,  14,   5,
        -35,  -8,  11,   2,   8,  15,  -3,   1,
         -1, -18,  -9,  -19, -30, -15, -60, -54
    };

    inline static const int king_mg_pst[64] = {
        -65,  23,  16, -15, -56, -34,   2,  13,
         29,  -1, -20,  -7,  -8,  -4, -38, -29,
         -9,  24,   2, -16, -20,   6,  22, -22,
        -17, -20, -12, -27, -30, -25, -14, -36,
        -49,  -1, -27, -39, -46, -44, -33, -51,
        -14, -14, -22, -46, -44, -30, -15, -27,
          1,   7,  -8, -64, -43, -16,   9,   8,
        -15,  36,  12, -54,   8, -28,  24,  14
    };

    inline static const int king_eg_pst[64] = {
        -74, -35, -18, -18, -11,  15,   4, -17,
        -12,  17,  14,  17,  17,  38,  23,  11,
         10,  17,  23,  15,  20,  45,  44,  13,
         -8,  22,  24,  27,  26,  33,  26,   3,
        -18,  -4,  21,  24,  21,  23,  12, -11,
        -19,  -3,  11,  21,  23,  16,   7,  -9,
        -27, -11,   4,  13,  14,   4,  -5, -17,
        -53, -34, -21, -11, -28, -14, -24, -43
    };
    // clang-format on

    static const int* getPstTable(PieceType pieceType) {
        switch (pieceType.value()) {
            case PieceTypes::PAWN.value():
                return pawn_pst;
            case PieceTypes::KNIGHT.value():
                return knight_pst;
            case PieceTypes::BISHOP.value():
                return bishop_pst;
            case PieceTypes::ROOK.value():
                return rook_pst;
            case PieceTypes::QUEEN.value():
                return queen_pst;
            default:
                return nullptr;
        }
    }
    /*
    static double evaluatePeSTO(Position& position) {
        int mg_score   = 0;
        int eg_score   = 0;
        int game_phase = 0;

        auto board = position.board();
        auto us    = position.us();

        for (Square sq : Squares::all()) {
            auto piece = board[sq.value()];
            if (piece == Pieces::NONE) continue;

            PieceType type  = piece.type();
            Color     color = piece.color();

            int pst_index = (color == Colors::WHITE) ? sq.value() : (sq.value() ^ 56);

            int mg_mat = mg_value[type.value()];
            int eg_mat = eg_value[type.value()];

            int mg_pos = 0;
            int eg_pos = 0;

            if (type == PieceTypes::KING) {
                mg_pos = king_mg_pst[pst_index];
                eg_pos = king_eg_pst[pst_index];
            } else {
                const int* table = getPstTable(type);
                if (table) {
                    mg_pos = table[pst_index];
                    eg_pos = table[pst_index];
                }
            }

            if (color == us) {
                mg_score += (mg_mat + mg_pos);
                eg_score += (eg_mat + eg_pos);
            } else {
                mg_score -= (mg_mat + mg_pos);
                eg_score -= (eg_mat + eg_pos);
            }

            game_phase += game_phase_weights[type.value()];
        }

        if (game_phase > 24) game_phase = 24;

        int final_score = (mg_score * game_phase + eg_score * (24 - game_phase)) / 24;

        return static_cast<double>(final_score) / 100.0;
    }
    */
};