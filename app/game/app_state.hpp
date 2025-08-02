
#pragma once

#include <atomic>
#include <cstddef>
#include <functional>
#include <mutex>
#include <optional>
#include <vector>

#include "game.hpp"

class AppState {
   public:
    void addGame(const std::function<void(Game&)>& initializer = {}) {
        std::lock_guard lock(m_mutex);
        m_games.emplace_back();
        auto& new_game = m_games.back();

        if (initializer) {
            initializer(new_game);
        }

        m_selected_index = m_games.size() - 1;
    }

    void selectGame(std::size_t index) {
        std::lock_guard lock(m_mutex);
        if (index < m_games.size()) {
            m_selected_index = index;
        }
    }

    void closeGame(size_t index) {
        std::lock_guard lock(m_mutex);
        if (index >= m_games.size()) return;

        m_games.erase(m_games.begin() + static_cast<int>(index));

        if (!m_selected_index) return;

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

    void applyToAllGames(const std::function<void(Game&)>& action) {
        std::lock_guard lock(m_mutex);
        for (auto& game : m_games) {
            action(game);
        }
    }

    void applyToCurrentGame(const std::function<void(Game&)>& action) {
        std::lock_guard lock(m_mutex);
        if (auto index = currentIndex()) {
            action(m_games[*index]);
        }
    }

    struct UISnapshot {
        std::vector<std::string>   game_titles;
        std::optional<std::size_t> selected_index;
        const Game*                current_game = nullptr;
    };

    UISnapshot snapshot() const {
        std::lock_guard lock(m_mutex);
        UISnapshot      snap;
        snap.game_titles.reserve(m_games.size());

        for (const auto& game : m_games) {
            snap.game_titles.push_back(game.title());
        }

        snap.selected_index = m_selected_index;

        if (auto index = currentIndex()) {
            snap.current_game = &m_games[*index];
        }
        return snap;
    }

    [[nodiscard]] bool shouldQuit() const { return m_quit_flag.load(); }
    void               signalQuit() { m_quit_flag = true; }

   private:
    std::optional<std::size_t> currentIndex() const {
        if (m_selected_index.has_value() && m_selected_index.value() < m_games.size()) {
            return m_selected_index;
        }
        return std::nullopt;
    }

    std::vector<Game>          m_games;
    std::optional<std::size_t> m_selected_index;
    mutable std::mutex         m_mutex;
    std::atomic<bool>          m_quit_flag{false};
};
