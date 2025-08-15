#pragma once

#include <thread>

#include "glfw_wrapper.hpp"
#include "imgui_wrapper.hpp"
#include "rect.hpp"
#include "resource_manager.hpp"
#include "session.hpp"

class Renderer {
   public:
    void start(int width, int height, const char *title, bool use_vsync, Session &session) {
        m_session = &session;
        m_thread  = std::thread([width, height, title, use_vsync, this /*, &app_state*/] {
            ResourceManager::init();
            initialize(width, height, title, use_vsync);
            renderLoop();
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
    void renderLoop() {
        while (!m_session->shouldQuit()) {
            updateTime();
            updateMousePosition();

            beginFrame();
            fillFrame();

            glClear(GL_COLOR_BUFFER_BIT);

            drawMainMenuBar();

            drawWorkspace();

            drawDemoWindow();

            finishFrame();

            if (m_glfw.windowShouldClose()) {
                m_session->signalQuit();
            }
        }
    }
    void terminate();

    void updateTime();
    void beginFrame();
    void fillFrame(float r = 0, float g = 0, float b = 0, float a = 1);
    void finishFrame();
    void toggleDemoWindow();

    void updateMousePosition() {
        m_mousePos.first  = ImGui::GetMousePos().x;
        m_mousePos.second = ImGui::GetMousePos().y;
    }
    [[nodiscard]] const std::pair<float, float> &getMousePosition() const { return m_mousePos; }

    void               initPieceTextures();
    static ImTextureID loadTexture(Resource resource);

    void drawMainMenuBar();
    void drawDemoWindow();
    void drawLostPieces();
    void drawGameInfo();
    void drawWorkspace();
    void drawGame();
    void drawBoard(Rect<float> board);
    void drawPieces(Rect<float> board);
    void drawGameUI(Rect<float> panel);

    bool m_justCreatedGame = false;

    GLFWWrapper  m_glfw;
    IMGUIWrapper m_imgui;

    std::unordered_map<Piece, ImTextureID> m_pieceTextures;

    std::pair<float, float> m_mousePos;

    // bool m_wasHoldingPiece = false;
    std::pair<float, float> m_lastHoldedPiecePosition = {0, 0};

    double m_lastTime       = 0;
    double m_deltaTime      = 0;
    double m_currentTime    = 0;
    bool   m_showDemoWindow = false;

    Session    *m_session = nullptr;
    std::thread m_thread;
};
