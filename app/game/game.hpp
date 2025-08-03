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

    void                      setTitle(const std::string& title) { m_title = title; }
    void                      setWhiteName(const std::string& name) { m_white_name = name; }
    void                      setBlackName(const std::string& name) { m_black_name = name; }
    [[nodiscard]] std::string title() const { return m_title.empty() ? whiteName() + " vs " + blackName() : m_title; }
    [[nodiscard]] std::string whiteName() const { return m_white_name.empty() ? "Player White" : m_white_name; }
    [[nodiscard]] std::string blackName() const { return m_black_name.empty() ? "Player Black" : m_black_name; }

    // get setters for string
    [[nodiscard]] const Position& position() const { return m_position; }

   private:
    Position    m_position;
    std::string m_title;
    std::string m_white_name;
    std::string m_black_name;
};