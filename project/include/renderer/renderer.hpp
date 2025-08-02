#pragma once

#include <thread>
#include <vector>

#include "app_state.hpp"
#include "game.hpp"
#include "glfw_wrapper.hpp"
#include "imgui_wrapper.hpp"
#include "resource_manager.hpp"

class Renderer {
   public:
    void start(int width, int height, const char *title, bool use_vsync, AppState &app_state) {
        m_app_state = &app_state;
        m_thread    = std::thread([width, height, title, use_vsync, this, &app_state] {
            ResourceManager::init();
            initialize(width, height, title, use_vsync);
            runLoop();
            terminate();
        });
    }
    void stop() {
        if (m_thread.joinable()) {
            m_thread.join();
        }
    }

   private:
    void initialize(int width, int height, const char *title, bool use_vsync);
    void runLoop() {
        while (!m_glfw.windowShouldClose() && !m_app_state->shouldQuit()) {
            m_app_state->render([this](const auto &games) {
                for (const auto &game : games) {
                    renderGame(game);
                }
            });
        }
        state_->signalQuit();
    }
    void terminate();

    void render();
    void updateTime();
    void beginFrame();
    void fillFrame(float r = 0, float g = 0, float b = 0, float a = 1);
    void finishFrame();
    void attachGames(std::vector<Game> &games) { m_games = &games; }
    void toggleDemoWindow();
    void updateMousePosition() {
        m_mousePos.first  = ImGui::GetMousePos().x;
        m_mousePos.second = ImGui::GetMousePos().y;
    }
    [[nodiscard]] const std::pair<float, float> &getMousePosition() const { return m_mousePos; }
    static ImTextureID                           loadTextureFromResources(const std::string &filename);
    void                                         loadTextures();

    void drawMainMenuBar();
    void drawDemoWindow();
    void drawLostPieces();
    void drawGameInfo();
    void drawWorkspace();
    void drawGame();

   private:
    std::vector<Game> *m_games;
    int                m_gameIndex       = 0;
    bool               m_justCreatedGame = false;

    GLFWWrapper  m_glfw;
    IMGUIWrapper m_imgui;

    std::array<ImTextureID, Piece::mask()> pieceTextures;

    std::pair<float, float> m_mousePos;

    // bool m_wasHoldingPiece = false;
    std::pair<float, float> m_lastHoldedPiecePosition = {0, 0};

    double m_lastTime       = 0;
    double m_deltaTime      = 0;
    double m_currentTime    = 0;
    bool   m_showDemoWindow = false;

    AppState   *m_app_state = nullptr;
    std::thread m_thread;
};
