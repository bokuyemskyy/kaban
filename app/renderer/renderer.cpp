#include "renderer.hpp"

#include <imgui.h>

#include <cstddef>
#include <stdexcept>
#include <string>

#include "navigation.hpp"
#include "piece.hpp"
#include "rect.hpp"
#include "resource_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

constexpr float LEFT_PANEL_RATIO     = 0.6;
constexpr float VERTICAL_BOARD_RATIO = 0.86;
constexpr float ITEM_SPACING         = 6;
constexpr int   BOARD_MARGIN         = 16;
constexpr int   PIECE_MARGIN         = 4;
constexpr ImU32 IM_WHITE             = IM_COL32(255, 255, 255, 255);
constexpr int   a_CHAR_INDEX         = 97;

void Renderer::initialize(int width, int height, const char *title, bool use_vsync) {
    m_glfw.initialize(width, height, title, use_vsync);
    m_imgui.initialize(m_glfw.window());

    initPieceTextures();

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
    ImVec2 panelMin  = ImGui::GetCursorScreenPos();
    ImVec2 availSize = ImGui::GetContentRegionAvail();

    Rect panel = Rect(panelMin.x, panelMin.y, availSize.x, availSize.y);

    const int   WINDOW_PADDING = ImGui::GetStyle().WindowPadding.x;
    const int   FONT_SIZE      = ImGui::GetIO().Fonts->Fonts[0]->FontSize;
    const float BOARD_SIZE =
        std::min(availSize.x, availSize.y) - static_cast<float>(2 * (FONT_SIZE + WINDOW_PADDING + BOARD_MARGIN));

    ImVec2 boardMin = ImVec2(panel.x + ((availSize.x - BOARD_SIZE) / 2), panel.y + ((availSize.y - BOARD_SIZE) / 2));
    Rect   board    = Rect(boardMin.x, boardMin.y, BOARD_SIZE, BOARD_SIZE);

    drawBoard(board);
    drawGameUI(panel);
    drawPieces(board);
}

void Renderer::drawBoard(Rect<float> board) {
    ImDrawList *drawList = ImGui::GetWindowDrawList();

    for (Square square : Square::all()) {
        ImU32 color = square.light() ? IM_COL32(240, 217, 181, 255) : IM_COL32(181, 136, 99, 255);
        Rect  rect  = square.normalizedRect().absolute(board.width, board.height);

        ImVec2 begin = ImVec2(board.x + rect.left(), board.y + rect.top());
        ImVec2 end   = ImVec2(board.x + rect.right(), board.y + rect.bottom());

        drawList->AddRectFilled(begin, end, color);
    }

    const ImVec2 CHAR_SIZE = ImGui::CalcTextSize("A");

    for (File file : File::all()) {
        std::string label(1, static_cast<char>(a_CHAR_INDEX + file));

        Square reference_square = Square(file, 0);
        Rect   reference_rect   = reference_square.normalizedRect().absolute(board.width, board.height);

        float x = board.x + reference_rect.left() + (reference_rect.width / 2) - (CHAR_SIZE.x / 2);
        drawList->AddText(ImVec2(x, board.y - CHAR_SIZE.y - PIECE_MARGIN), IM_WHITE, label.c_str());
        drawList->AddText(ImVec2(x, board.y + board.height + PIECE_MARGIN), IM_WHITE, label.c_str());
    }
    for (Rank rank : Rank::all()) {
        std::string label = std::to_string(Rank::NB - rank);

        Square reference_square = Square(0, rank);
        Rect   reference_rect   = reference_square.normalizedRect().absolute(board.width, board.height);

        float y = board.y + reference_rect.top() + (reference_rect.height / 2) - (CHAR_SIZE.y / 2);
        drawList->AddText(ImVec2(board.x - CHAR_SIZE.x - PIECE_MARGIN, y), IM_WHITE, label.c_str());
        drawList->AddText(ImVec2(board.x + board.width + PIECE_MARGIN, y), IM_WHITE, label.c_str());
    }
}

void Renderer::drawGameUI(Rect<float> panel) {
    ImDrawList *drawList = ImGui::GetWindowDrawList();

    const int FONT_SIZE = ImGui::GetIO().Fonts->Fonts[0]->FontSize;

    auto game = m_session->selectedGame();

    if (game) {
        drawList->AddText(ImVec2(panel.x, panel.y), IM_WHITE, game.value()->blackName().c_str());
        drawList->AddText(ImVec2(panel.x, panel.bottom() - static_cast<float>(FONT_SIZE)), IM_WHITE,
                          game.value()->whiteName().c_str());
    }
}

void Renderer::drawPieces(Rect<float> board) {
    ImDrawList *drawList = ImGui::GetWindowDrawList();

    auto game = m_session->selectedGame();

    if (game) {
        for (Square square : Square::all()) {
            Rect rect = square.normalizedRect().absolute(board.width, board.height);

            ImVec2 begin = ImVec2(board.x + rect.left(), board.y + rect.top());
            ImVec2 end   = ImVec2(board.x + rect.right(), board.y + rect.bottom());

            Piece piece = game.value()->snapshot().position[square];

            ImVec2 piece_begin = ImVec2(begin.x + PIECE_MARGIN, begin.y + PIECE_MARGIN);
            ImVec2 piece_end   = ImVec2(end.x - PIECE_MARGIN, end.y - PIECE_MARGIN);

            if (piece != Piece::NONE) {
                drawList->AddImage(pieceTextures[piece], piece_begin, piece_end);
            }
        }
    }
}

void Renderer::drawLostPieces() { ImGui::Text("Captured Pieces"); }

void Renderer::drawGameInfo() {
    ImGui::Text("Game Information");

    ImGui::Separator();

    ImGui::Text("Game status:");
    ImGui::Text("  Turn: White to move");
    ImGui::Text("  Result: In progress");

    ImGui::Separator();

    ImGui::Text("Engine evaluation (N/A):");
    ImGui::Text("  Score: NAN");
    ImGui::Text("  Best move: NAN");
    ImGui::Text("  Depth: NAN");

    ImGui::Separator();

    ImGui::Text("Move history:");
    ImGui::Text("  1. ");

    ImGui::Separator();
}

void Renderer::drawWorkspace() {
    auto session_snapshot = m_session->snapshot();

    ImGui::GetStyle().ItemSpacing.y = ITEM_SPACING;
    ImGui::GetStyle().ItemSpacing.x = ITEM_SPACING;
    ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
    ImGui::SetNextWindowSize(
        ImVec2(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y - ImGui::GetFrameHeight()));

    ImGui::Begin("Workspace", nullptr,
                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoBringToFrontOnFocus);

    if (ImGui::BeginTabBar("GameTabs")) {
        for (size_t game_index = 0; game_index < session_snapshot.games.size(); ++game_index) {
            ImGuiTabItemFlags flags = 0;
            if (m_justCreatedGame && session_snapshot.selected_index == game_index) {
                flags |= ImGuiTabItemFlags_SetSelected;
                m_justCreatedGame = false;
            }

            std::string label = session_snapshot.games[game_index].title + "###" + std::to_string(game_index);
            if (ImGui::BeginTabItem(label.c_str(), nullptr, flags)) {
                ImGui::EndTabItem();
            }
        }
        if (ImGui::TabItemButton("+", ImGuiTabItemFlags_Trailing | ImGuiTabItemFlags_NoTooltip)) {
            m_session->addGame();
            m_justCreatedGame = true;
        }
        ImGui::EndTabBar();
    }

    ImVec2 availSpace     = ImGui::GetContentRegionAvail();
    float  leftPanelWidth = availSpace.x * LEFT_PANEL_RATIO;
    // float  rightPanelWidth  = availSpace.x - leftPanelWidth;
    float boardHeight = availSpace.y * VERTICAL_BOARD_RATIO;
    // float  lostPiecesHeight = availSpace.y - boardHeight;

    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, availSpace.y), 0);
    {
        ImGui::BeginChild("Game", ImVec2(0, boardHeight), 1);
        drawGame();
        ImGui::EndChild();

        ImGui::BeginChild("LostPieces", ImVec2(0, 0), 1);
        drawLostPieces();
        ImGui::EndChild();
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginChild("RightPanel", ImVec2(0, availSpace.y), 1);
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

void Renderer::terminate() {
    m_imgui.terminate();
    m_glfw.terminate();
}

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
    m_deltaTime   = m_currentTime - m_lastTime;
    m_lastTime    = m_currentTime;
}

void Renderer::fillFrame(float r, float g, float b, float a) { m_glfw.fillFrame(r, g, b, a); }

void Renderer::initPieceTextures() {
    for (Piece piece : Piece::all()) {
        std::string name(1, piece.to_char());
        GLuint      texture = loadTexture(ResourceManager::getResource(name + ".png"));
        if (texture == 0) throw std::runtime_error("Failed to load a texture");
        pieceTextures[piece] = texture;
    }
}

ImTextureID Renderer::loadTexture(Resource resource) {
    GLuint texture{};
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    int            width{};
    int            height{};
    int            channels{};
    unsigned char *data =
        stbi_load_from_memory(resource.data, static_cast<int>(resource.length), &width, &height, &channels, 4);

    if (data == nullptr) {
        throw std::runtime_error(stbi_failure_reason());
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