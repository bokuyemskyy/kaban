#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <GLFW/glfw3.h>

#include <game.hpp>
#include <glfw_wrapper.hpp>
#include <imgui_wrapper.hpp>
#include <layout_manager.hpp>
#include <position.hpp>
#include <unordered_map>
#include <utility>

namespace Ratios {
constexpr float INTERFACE_WIDTH = 0.4F;
constexpr float GAME_WIDTH = 0.6F;
}  // namespace Ratios

namespace Sectors {
constexpr auto INTERFACE = "Interface";
constexpr auto GAME = "Game";
}  // namespace Sectors

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

    void toggleDemoWindow();
    bool windowShouldClose();
    void hookUpGame(Game *game);
    void updateMousePosition() {
        m_mousePos.first = ImGui::GetMousePos().x;
        m_mousePos.second = ImGui::GetMousePos().y;
    }
    const std::pair<float, float> &getMousePosition() const { return m_mousePos; }

    LayoutManager *getLayoutManager();

    bool loadTextures();
    void drawGame();
    void drawSquare(float x, float y, float width, float height, float r, float g, float b);
    void drawImage(float x, float y, float width, float height, GLuint texture);
    void drawPiece(float x, float y, float width, float height, float padding, GLuint texture);

   private:
    LayoutManager m_layoutManager;

    Game *m_game;
    GLFWWrapper m_glfw;
    IMGUIWrapper m_imgui;

    std::unordered_map<Piece, GLuint> pieceTextures;

    std::pair<float, float> m_mousePos;

    bool m_wasHoldingPiece = false;
    std::pair<float, float> m_lastHoldedPiecePosition = {0, 0};

    double m_lastTime = 0;
    double m_deltaTime = 0;
    double m_currentTime = 0;
    bool m_showDemoWindow = false;
};

#endif