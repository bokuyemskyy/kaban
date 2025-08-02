#ifndef APP_STATE
#define APP_STATE

#include <atomic>
#include <functional>
#include <mutex>
#include <vector>

#include "game.hpp"

class GameState {
   public:
    void addGame() {
        std::lock_guard lock(m_mutex);
        m_games.emplace_back();
    }

    void applyToAllGames(const std::function<void(Game&)>& action) {
        std::lock_guard lock(m_mutex);
        for (auto& game : m_games) {
            action(game);
        }
    }

    void renderGames(const std::function<void(const std::vector<Game>&)>& renderer) {
        std::lock_guard lock(m_mutex);
        renderer(m_games);
    }

    [[nodiscard]] bool shouldQuit() const { return m_quit_flag.load(); }

    void signalQuit() { m_quit_flag = true; }

   private:
    std::vector<Game> m_games;
    std::mutex        m_mutex;
    std::atomic<bool> m_quit_flag{false};
};

#endif