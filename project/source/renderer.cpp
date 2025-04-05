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

#include "constants.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "resource_manager.hpp"
#include "types.hpp"
#include "utils.hpp"

constexpr int BOARD_MARGIN = 16;
constexpr int PIECE_MARGIN = 4;
constexpr ImU32 IM_WHITE = IM_COL32(255, 255, 255, 255);
constexpr int a_CHAR = 97;

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

void Renderer::drawGame() {
    ImVec2 gameStartPos = ImGui::GetCursorScreenPos();
    ImVec2 availSize = ImGui::GetContentRegionAvail();
    ImDrawList *drawList = ImGui::GetWindowDrawList();

    const int WINDOW_PADDING = ImGui::GetStyle().WindowPadding.x;
    const int FONT_SIZE = ImGui::GetIO().Fonts->Fonts[0]->FontSize;

    const float PX_BOARD_SIZE = std::min(availSize.x, availSize.y) -
                                static_cast<float>(2 * (FONT_SIZE + WINDOW_PADDING + BOARD_MARGIN));
    const float SQUARE_SIZE = PX_BOARD_SIZE / BOARD_SIZE;
    const ImVec2 CHAR_SIZE = ImGui::CalcTextSize("A");

    drawList->AddText(ImVec2(gameStartPos.x, gameStartPos.y), IM_WHITE, "Player Black");
    drawList->AddText(
        ImVec2(gameStartPos.x, gameStartPos.y + availSize.y - static_cast<float>(FONT_SIZE)),
        IM_WHITE, "Player White");

    ImVec2 boardStartPos = ImVec2(gameStartPos.x + ((availSize.x - PX_BOARD_SIZE) / 2),
                                  gameStartPos.y + ((availSize.y - PX_BOARD_SIZE) / 2));

    for (int row = 0; row < BOARD_SIZE; row++) {
        for (int col = 0; col < BOARD_SIZE; col++) {
            ImVec2 squarePos = boardStartPos;
            squarePos.x += col * SQUARE_SIZE;
            squarePos.y += row * SQUARE_SIZE;

            bool isLightSquare = (row + col) % 2 == 0;
            ImU32 squareColor =
                isLightSquare ? IM_COL32(240, 217, 181, 255) : IM_COL32(181, 136, 99, 255);

            drawList->AddRectFilled(squarePos,
                                    ImVec2(squarePos.x + SQUARE_SIZE, squarePos.y + SQUARE_SIZE),
                                    squareColor);

            Piece piece = WPAWN;
            if (piece != EMPTY) {
                int textureIndex = piece;
                drawList->AddImage(pieceTextures.at(piece),
                                   ImVec2(squarePos.x + PIECE_MARGIN, squarePos.y + PIECE_MARGIN),
                                   ImVec2(squarePos.x + SQUARE_SIZE - PIECE_MARGIN,
                                          squarePos.y + SQUARE_SIZE - PIECE_MARGIN));
            }
        }
    }

    for (int row = 0; row < BOARD_SIZE; row++) {
        const char *label = std::to_string(8 - row).c_str();

        drawList->AddText(
            ImVec2(boardStartPos.x - CHAR_SIZE.x - PIECE_MARGIN,
                   boardStartPos.y + (row * SQUARE_SIZE) + (SQUARE_SIZE / 2) - (CHAR_SIZE.y / 2)),
            IM_WHITE, label);
        drawList->AddText(
            ImVec2(boardStartPos.x + PX_BOARD_SIZE + PIECE_MARGIN,
                   boardStartPos.y + (row * SQUARE_SIZE) + (SQUARE_SIZE / 2) - (CHAR_SIZE.y / 2)),
            IM_WHITE, label);
    }
    for (int col = 0; col < BOARD_SIZE; col++) {
        const char label[] = {static_cast<char>(a_CHAR + col), '\0'};

        drawList->AddText(
            ImVec2(boardStartPos.x + (col * SQUARE_SIZE) + (SQUARE_SIZE / 2) - (CHAR_SIZE.x / 2),
                   boardStartPos.y - CHAR_SIZE.y - PIECE_MARGIN),
            IM_WHITE, label);
        drawList->AddText(
            ImVec2(boardStartPos.x + (col * SQUARE_SIZE) + (SQUARE_SIZE / 2) - (CHAR_SIZE.x / 2),
                   boardStartPos.y + PX_BOARD_SIZE + PIECE_MARGIN),
            IM_WHITE, label);
    }
}

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

void Renderer::loadTextures() {
    for (uint8_t i = 0; i < 12; i++) {
        std::string name(1, PieceToFEN.at(Piece(i)));
        GLuint texture = loadTextureFromResources(name + ".png");
        if (texture == 0) ErrorBus::handleError(0, "Failed to load a texture");
        pieceTextures.emplace(Piece(i), texture);
    }
}

ImTextureID Renderer::loadTextureFromResources(const std::string &filename) {
    GLuint texture{};
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int width{};
    int height{};
    int channels{};
    Resource resource = ResourceManager::getResource(filename);
    unsigned char *data = stbi_load_from_memory(resource.first, static_cast<int>(resource.second),
                                                &width, &height, &channels, 4);

    if (data == 0) {
        ErrorBus::handleError(1, stbi_failure_reason());
        return 0;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stbi_image_free(data);

    return ImTextureID(texture);
}