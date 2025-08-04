#pragma once

#include <atomic>
#include <cstddef>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <vector>

#include "game.hpp"

class Session {
   public:
    void addGame() {
        std::lock_guard lock(m_mutex);

        m_games.emplace_back(std::make_shared<Game>(m_next_game_id++));

        m_selected_index = m_games.size() - 1;
        m_dirty          = true;
    }

    void selectGame(size_t index) {
        std::lock_guard lock(m_mutex);

        if (index >= m_games.size()) return;

        m_selected_index = index;

        m_dirty = true;
    }

    void closeGame(size_t index) {
        std::lock_guard lock(m_mutex);

        if (index >= m_games.size()) return;

        m_games.erase(m_games.begin() + static_cast<int>(index));

        if (m_selected_index) {
            if (index < *m_selected_index) {
                --(*m_selected_index);
            } else if (index == *m_selected_index) {
                if (m_games.empty()) {
                    m_selected_index.reset();
                } else if (index >= m_games.size()) {
                    m_selected_index = m_games.size() - 1;
                }
            }
        }

        m_dirty = true;
    }

    std::optional<std::shared_ptr<Game>> selectedGame() {
        if (!m_selected_index) return std::nullopt;
        return m_games[*m_selected_index];
    }

    struct Snapshot {
        struct GameEntry {
            size_t      id;
            std::string title;
        };
        Snapshot() = default;
        Snapshot(const std::vector<std::shared_ptr<Game>>& games_, const std::optional<size_t> selected_index_)
            : selected_index(selected_index_) {
            games.reserve(games_.size());
            for (const auto& game : games_) {
                games.emplace_back(GameEntry{.id = game->id(), .title = game->title()});
            }
        }

        std::vector<GameEntry>     games;
        std::optional<std::size_t> selected_index;

        [[nodiscard]] constexpr bool empty() const { return games.empty(); }
    };

    Snapshot snapshot() const {
        std::lock_guard lock(m_mutex);

        if (m_dirty) {
            m_cached_snapshot = Snapshot(m_games, m_selected_index);
            m_dirty           = false;
        }

        return m_cached_snapshot;
    }

    [[nodiscard]] bool shouldQuit() const { return m_quit_flag.load(); }
    void               signalQuit() { m_quit_flag = true; }

   private:
    mutable std::mutex m_mutex;

    std::vector<std::shared_ptr<Game>> m_games;
    std::optional<size_t>              m_selected_index;

    std::atomic<size_t> m_next_game_id{0};

    mutable Snapshot          m_cached_snapshot;
    mutable std::atomic<bool> m_dirty{true};

    std::atomic<bool> m_quit_flag{false};
};
