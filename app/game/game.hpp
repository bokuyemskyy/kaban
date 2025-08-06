#pragma once

#include <array>
#include <atomic>
#include <cstddef>
#include <mutex>
#include <string>

#include "move.hpp"
#include "navigation.hpp"
#include "piece.hpp"
#include "position.hpp"

class Game {
   public:
    Game(size_t id) : m_id(id) {};

    struct Snapshot {
        Snapshot() = default;
        Snapshot(const Position& position_) {
            for (Square square : Square::all()) {
                board[square] = position_.at(square);
            }
        }
        std::array<Piece, Square::number()> board;
    };

    [[nodiscard]] const Snapshot& snapshot() const {
        std::lock_guard lock(m_mutex);

        if (m_dirty) {
            m_cached_snapshot = Snapshot(m_position);
            m_dirty           = false;
        }

        return m_cached_snapshot;
    }

    void clear() { m_position.setFromFEN(); }
    void start() { m_position.setFromFEN("8/8/8/8/8/8/8/8"); }

    void doMove(const std::string /*moveString*/) {
        m_position.doMove(Move());
        m_dirty = true;
    }
    void doMove(int /*from*/, int /*to*/) {
        m_position.doMove(Move());
        m_dirty = true;
    }
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
    [[nodiscard]] size_t      id() const { return m_id; }

   private:
    mutable std::mutex m_mutex;

    size_t      m_id;
    Position    m_position;
    std::string m_title;
    std::string m_white_name;
    std::string m_black_name;

    mutable Snapshot          m_cached_snapshot;
    mutable std::atomic<bool> m_dirty{true};
};