#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <unordered_map>
#include <vector>

#include "game.hpp"
#include "glfw_wrapper.hpp"
#include "imgui_wrapper.hpp"

class Renderer {
   public:
    Renderer() = default;
    ~Renderer() = default;

    void initialize(Dimensions dimensions, const char *title, bool useVsync);
    void terminate();

    void render();
    void updateTime();
    void beginFrame();
    void fillFrame(float r = 0, float g = 0, float b = 0, float a = 1);
    void finishFrame();
    void attachGames(std::vector<Game> &games) { m_games = &games; }
    void toggleDemoWindow();
    bool windowShouldClose();
    void updateMousePosition() {
        m_mousePos.first = ImGui::GetMousePos().x;
        m_mousePos.second = ImGui::GetMousePos().y;
    }
    const std::pair<float, float> &getMousePosition() const { return m_mousePos; }
    ImTextureID loadTextureFromResources(const std::string &filename);
    void loadTextures();
    void drawMainMenuBar();
    void drawDemoWindow();
    void drawLostPieces();
    void drawGameInfo();
    void drawWorkspace();
    void drawGame();

   private:
    std::vector<Game> *m_games;
    int m_gameIndex = 0;
    bool m_justCreatedGame = false;

    GLFWWrapper m_glfw;
    IMGUIWrapper m_imgui;

    std::unordered_map<Piece, ImTextureID> pieceTextures;

    std::pair<float, float> m_mousePos;

    bool m_wasHoldingPiece = false;
    std::pair<float, float> m_lastHoldedPiecePosition = {0, 0};

    double m_lastTime = 0;
    double m_deltaTime = 0;
    double m_currentTime = 0;
    bool m_showDemoWindow = false;
};

#endif