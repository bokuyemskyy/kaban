#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <cmath>
#include <cstdint>
#include <error_handler.hpp>
#include <game.hpp>
#include <layout_manager.hpp>
#include <navigation.hpp>
#include <renderer.hpp>
#include <string>
#include <texture_loader.hpp>
#include <utility>

#include "constants.hpp"
#include "types.hpp"
#include "utils.hpp"

bool Renderer::init(float height, const char *title) {
    float width = height * (5.0f / 3.0f);

    m_layoutManager.defineSector("Interface", 0, 0, width * (2.0f / 5.0f), height);
    m_layoutManager.defineSector("Game", width * (2.0f / 5.0f), 0, width * (3.0f / 5.0f), height);

    if (!m_glfw.init(width, height, title, true) || !m_imgui.init(m_glfw.getWindow()) ||
        !loadTextures()) {
        shutdown();
        handleError(1, "Failed to init graphics");
        return false;
    }

    updateTime();

    return true;
};

void Renderer::render() {
    updateTime();
    updateMousePosition();

    newFrame();
    fillFrame();

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1000, 0, 600, -1, 1);

    ImGui::SetNextWindowSize(ImVec2(m_layoutManager.getSector("Interface").width,
                                    m_layoutManager.getSector("Interface").height));
    ImGui::SetNextWindowPos(
        ImVec2(m_layoutManager.getSector("Interface").x, m_layoutManager.getSector("Interface").y));
    ImGui::Begin("Game", nullptr,
                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    ImGui::Text("Hello, welcome to the Kaban!");
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    if (ImGui::Button("Start a new game")) {
        if (m_game) {
            m_game->setFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        }
    }
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    ImGui::Separator();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    ImGui::Text("Current turn: %s", m_game->getTurn() == WHITE ? "White" : "Black");
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    std::string selectedPieceStr = "None";
    if (m_game->getSelectedSquare() != NO_SQ) {
        std::string ranks[] = {"A", "B", "C", "D", "E", "F", "G", "H"};
        int row = m_game->getSelectedSquare() / 8;
        int col = m_game->getSelectedSquare() % 8;
        selectedPieceStr = ranks[col] + std::to_string(8 - row);
    }
    ImGui::Text("Selected piece: %s", selectedPieceStr.c_str());
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    std::string isHoldingPieceText = "";
    if (m_game->isHoldingPiece()) {
        isHoldingPieceText = "true";
    } else {
        isHoldingPieceText = "false";
    }
    ImGui::Text("Is holding a piece: %s", isHoldingPieceText.c_str());
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
    ImGui::End();

    if (m_game) {
        drawGame();
    }

    if (m_showDemoWindow) {
        m_imgui.showDemoWindow();
        m_imgui.keepWindowInBounds("Dear ImGui Demo");
    }

    finishFrame();
}

void Renderer::shutdown() {
    m_imgui.shutdown();
    m_glfw.shutdown();
}

bool Renderer::windowShouldClose() { return m_glfw.windowShouldClose(); }

void Renderer::toggleDemoWindow() { m_showDemoWindow = !m_showDemoWindow; }

void Renderer::newFrame() {
    m_glfw.newFrame();
    m_imgui.newFrame();
}

void Renderer::finishFrame() {
    m_imgui.finishFrame();
    m_glfw.finishFrame();
}

void Renderer::updateTime() {
    m_currentTime = m_glfw.getTime();
    m_deltaTime = m_currentTime - m_lastTime;
    m_lastTime = m_currentTime;
}

void Renderer::fillFrame(float r, float g, float b, float a) { m_glfw.fillFrame(r, g, b, a); }

void Renderer::hookUpGame(Game *game) { m_game = game; }

bool Renderer::loadTextures() {
    for (uint8_t i = 0; i < 12; i++) {
        std::string name(1, PieceToFEN.at(Piece(i)));
        GLuint texture = TextureLoader::loadTexture(name + ".png");
        if (texture == 0) return false;
        pieceTextures[Piece(i)] = texture;
    }
    return true;
}

void Renderer::drawGame() {
    glClear(GL_COLOR_BUFFER_BIT);

    const Sector &boardSector = m_layoutManager.getSector("Game");
    float squareSize = boardSector.width / 8.0f;
    auto board = m_game->getMatrixBoard();

    for (uint8_t square = 0; square < 64; square++) {
        float yPos = boardSector.y + (square / 8) * squareSize;
        float xPos = boardSector.x + (square % 8) * squareSize;

        uint8_t relativeSquare = getTurnSquare(Square(square), m_game->getTurn());

        bool isDark = ((square >> 3) + (square % 8)) % 2 == 0;

        float colorValue = isDark ? 0.3f : 0.7f;

        drawSquare(xPos, yPos, squareSize, squareSize, colorValue, colorValue, colorValue);

        if (relativeSquare == m_game->getSelectedSquare()) {
            drawSquare(xPos, yPos, squareSize, squareSize, 0.7f, 0.3f, 0.3f);
        }

        Piece piece = board[(relativeSquare >> 3)][(relativeSquare % 8)];

        if (piece != EMPTY &&
            !((relativeSquare == m_game->getSelectedSquare() && m_game->isHoldingPiece()))) {
            drawPiece(xPos, yPos, squareSize, squareSize, 4, pieceTextures[piece]);
        }
        if (m_game->isHoldingPiece()) {
            std::pair<float, float> holdingPiecePos;
            float lerpSpeed = 0.5f * m_deltaTime;
            float halfSquareSize = squareSize / 2.0f;

            if (!m_wasHoldingPiece) {
                m_wasHoldingPiece = true;
                holdingPiecePos.first = m_mousePos.first - halfSquareSize;
                holdingPiecePos.second =
                    (static_cast<float>(m_glfw.getHeight()) - m_mousePos.second) - halfSquareSize;
            } else {
                float distance = Utils::distance(m_lastHoldedPiecePosition, m_mousePos);
                if (distance < 0.1f) {
                    holdingPiecePos = m_lastHoldedPiecePosition;
                } else {
                    holdingPiecePos.first =
                        m_lastHoldedPiecePosition.first +
                        (m_mousePos.first - halfSquareSize - m_lastHoldedPiecePosition.first) *
                            lerpSpeed;
                    holdingPiecePos.second =
                        m_lastHoldedPiecePosition.second +
                        ((static_cast<float>(m_glfw.getHeight()) - m_mousePos.second) -
                         halfSquareSize - m_lastHoldedPiecePosition.second) *
                            lerpSpeed;
                }
            }

            uint8_t selectedPiece = m_game->getSelectedSquare();
            drawPiece(holdingPiecePos.first, holdingPiecePos.second, squareSize, squareSize, 4,
                      pieceTextures[board[selectedPiece >> 3][selectedPiece % 8]]);

            m_lastHoldedPiecePosition = holdingPiecePos;
        } else {
            m_wasHoldingPiece = false;
        }
    }
}

void Renderer::drawSquare(float x, float y, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();
}

void Renderer::drawImage(float x, float y, float width, float height, GLuint texture) {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    glColor3f(1, 1, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + width, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + width, y + height);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + height);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

LayoutManager *Renderer::getLayoutManager() { return &m_layoutManager; }

void Renderer::drawPiece(float x, float y, float width, float height, float padding,
                         GLuint texture) {
    drawImage(x + padding, y + padding, width - 2 * padding, height - 2 * padding, texture);
}