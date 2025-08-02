#pragma once

#include <string>

#include "position.hpp"

class Game {
   public:
    void start() { m_position.setFromFEN(); }
    void reset() { m_position.setFromFEN("8/8/8/8/8/8/8/8"); }

    void doMove(const std::string /*moveString*/) { m_position.doMove(Move()); }
    void doMove(int /*from*/, int /*to*/) { m_position.doMove(Move()); }
    // dummy wrapper for the position
    // ensures encapsulation without game.getPosition().doMove()
    // the same should be implemented with undo

    void perft(int /*depth*/) {}
    // get setters for string
    [[nodiscard]] std::string title() const { return m_title; }

   private:
    Position                            m_position;
    std::string                         m_title;
    std::pair<std::string, std::string> m_opponents;
};