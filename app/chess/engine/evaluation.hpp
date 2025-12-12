#pragma once

#include "position.hpp"

class Evaluation {
   public:
    inline static const double MATE_SCORE = -10000.0;

    static double evaluatePosition(Position& position) {
        if (position.isGameOver()) {
            if (position.isCheckmate()) {
                return MATE_SCORE;
            }
            if (position.isStalemate()) {
                return 0.0;
            }
        }

        return evaluateMaterial(position);
    }

   private:
    inline static const std::unordered_map<PieceType, double> pieceValues = {
        {PieceTypes::PAWN, 1.0}, {PieceTypes::KNIGHT, 3.0}, {PieceTypes::BISHOP, 3.25},
        {PieceTypes::ROOK, 5.0}, {PieceTypes::QUEEN, 9.0},  {PieceTypes::KING, 0.0}};
    inline static const std::array<std::array<double, PieceTypes::count()>, Squares::count()> pieceSquareTables = {{}};

    static double evaluateMaterial(Position& position) {
        double material = 0;
        auto   board    = position.board();
        auto   us       = position.us();
        for (auto& piece : board) {
            if (piece == Pieces::NONE) continue;
            material += pieceValues.at(piece.type()) * (piece.color() == us ? 1 : -1);
        }
        return material;
    }
};