#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include <cmath>
#include <cstdint>
#include <error_bus.hpp>
#include <game.hpp>
#include <navigation.hpp>
#include <renderer.hpp>
#include <string>
#include <texture_loader.hpp>

#include "types.hpp"
#include "utils.hpp"

void Renderer::initialize(Dimensions dimensions, const char *title, bool useVsync) {
    m_glfw.initialize(dimensions, title, useVsync);
    m_imgui.initialize(m_glfw.window());

    loadTextures();

    updateTime();
}
void Renderer::drawMainMenuBar() {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New Game")) {
            }
            if (ImGui::MenuItem("Open Game")) {
            }
            if (ImGui::MenuItem("Save Game")) {
            }
            if (ImGui::MenuItem("Exit")) {
                m_glfw.setWindowShouldClose(true);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Undo Move")) {
            }
            if (ImGui::MenuItem("Redo Move")) {
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Games")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Commands")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Database")) {
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Help")) {
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void Renderer::drawGame() { ImGui::Text("Chess Board"); }

void Renderer::drawLostPieces() { ImGui::Text("Captured Pieces"); }

void Renderer::drawGameInfo() { ImGui::Text("Game Information"); }

void Renderer::drawWorkspace() {
    ImGui::GetStyle().ItemSpacing.y = 6.0f;
    ImGui::GetStyle().ItemSpacing.x = 6.0f;
    // Set main workspace window position and size
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x,
                                    ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()));

    // Begin main workspace window
    ImGui::Begin("Workspace", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoBringToFrontOnFocus);

    // Tab bar for game selection
    if (ImGui::BeginTabBar("GameTabs")) {
        if (ImGui::BeginTabItem("Game 1")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Game 2")) {
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("+")) {
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }

    // Calculate available space and division sizes
    ImVec2 availSpace = ImGui::GetContentRegionAvail();
    float leftPanelWidth = availSpace.x * 0.66f;
    float rightPanelWidth = availSpace.x - leftPanelWidth;
    float boardHeight = availSpace.y * 0.8f;
    float lostPiecesHeight = availSpace.y - boardHeight;

    // Left panel (containing game board and lost pieces)
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, availSpace.y), false);
    {
        // Game board area
        ImGui::BeginChild("Game", ImVec2(0, boardHeight), true);
        drawGame();
        ImGui::EndChild();

        // Lost pieces area
        ImGui::BeginChild("LostPieces", ImVec2(0, 0), true);
        drawLostPieces();
        ImGui::EndChild();
    }
    ImGui::EndChild();

    // Right panel (game info)
    ImGui::SameLine();
    ImGui::BeginChild("RightPanel", ImVec2(0, availSpace.y), true);
    {
        drawGameInfo();
    }
    ImGui::EndChild();

    ImGui::End();
}
void Renderer::drawDemoWindow() {
    if (m_showDemoWindow) {
        m_imgui.showDemoWindow();
        m_imgui.keepWindowInBounds("Dear ImGui Demo");
    }
}
void Renderer::render() {
    updateTime();
    updateMousePosition();

    beginFrame();
    fillFrame();

    glClear(GL_COLOR_BUFFER_BIT);

    drawMainMenuBar();
    drawWorkspace();

    drawDemoWindow();

    finishFrame();
}

void Renderer::terminate() {
    m_imgui.terminate();
    m_glfw.terminate();
}

bool Renderer::windowShouldClose() { return m_glfw.windowShouldClose(); }

void Renderer::toggleDemoWindow() { m_showDemoWindow = !m_showDemoWindow; }

void Renderer::beginFrame() {
    m_glfw.beginFrame();
    m_imgui.beginFrame();
}

void Renderer::finishFrame() {
    m_imgui.finishFrame();
    m_glfw.finishFrame();
}
void Renderer::updateTime() {
    m_currentTime = GLFWWrapper::time();
    m_deltaTime = m_currentTime - m_lastTime;
    m_lastTime = m_currentTime;
}

void Renderer::fillFrame(float r, float g, float b, float a) { m_glfw.fillFrame(r, g, b, a); }

void Renderer::hookUpGame(Game *game) { m_game = game; }

void Renderer::loadTextures() {
    for (uint8_t i = 0; i < 12; i++) {
        std::string name(1, PieceToFEN.at(Piece(i)));
        GLuint texture = TextureLoader::loadTexture(name + ".png");
        if (texture == 0) ErrorBus::handleError(0, "Failed to load a texture");
        pieceTextures[Piece(i)] = texture;
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

void Renderer::drawPiece(float x, float y, float width, float height, float padding,
                         GLuint texture) {
    drawImage(x + padding, y + padding, width - 2 * padding, height - 2 * padding, texture);
}