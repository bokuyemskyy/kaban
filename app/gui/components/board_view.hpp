#include <imgui.h>

#include <cmath>

#include "engine.hpp"
#include "gui_component.hpp"
#include "gui_context.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "square.hpp"
#include "texture_manager.hpp"

constexpr int   a_CHAR_INDEX = 97;
constexpr int   BOARD_MARGIN = 16;
constexpr int   PIECE_MARGIN = 4;
constexpr ImU32 IM_WHITE     = IM_COL32(255, 255, 255, 255);
constexpr ImU32 IM_REDDISH   = IM_COL32(255, 80, 80, 255);

class BoardView : IGuiComponent {
   public:
    using IGuiComponent::IGuiComponent;

    void draw() override {
        handle_input();

        Rect<float>  panel   = calculate_panel();
        BoardMetrics metrics = board_metrics(panel);

        squares(metrics);
        highlights(metrics);
        labels(metrics);
        player_names(panel);
        pieces(metrics);
    }

   private:
    struct BoardMetrics {
        ImVec2 origin;
        float  size;
        float  squareSize;
    };

    Rect<float> calculate_panel() {
        ImVec2 panelMin  = ImGui::GetCursorScreenPos();
        ImVec2 availSize = ImGui::GetContentRegionAvail();
        return Rect(panelMin.x, panelMin.y, availSize.x, availSize.y);
    }

    BoardMetrics board_metrics(const Rect<float>& panel) {
        const int WINDOW_PADDING = ImGui::GetStyle().WindowPadding.x;
        const int FONT_SIZE      = ImGui::GetIO().Fonts->Fonts[0]->FontSize;
        ImVec2    availSize(panel.width, panel.height);

        float  boardSize = std::min(availSize.x, availSize.y) - 2.0f * (FONT_SIZE + WINDOW_PADDING + BOARD_MARGIN);
        ImVec2 origin(panel.x + (availSize.x - boardSize) / 2, panel.y + (availSize.y - boardSize) / 2);

        return BoardMetrics{.origin = origin, .size = boardSize, .squareSize = boardSize / 8.0f};
    }

    void handle_input() {
        ImGuiIO& io = m_context.io;
        if (!ImGui::IsWindowHovered() || !ImGui::IsMouseClicked(0)) return;

        Rect<float>  panel   = calculate_panel();
        BoardMetrics metrics = board_metrics(panel);

        ImVec2 mouse = io.MousePos;
        float  relX  = mouse.x - metrics.origin.x;
        float  relY  = mouse.y - metrics.origin.y;

        File file = File(std::floor(relX / metrics.squareSize));
        Rank rank = Rank(7 - std::floor(relY / metrics.squareSize));

        if (file.has_value() && rank.has_value() && m_context.engine.at(Square(file, rank)) != Pieces::NONE) {
            m_context.state.selectedSquare = Square(file, rank);
        } else {
            m_context.state.selectedSquare = Squares::NONE;
        }
    }

    void squares(const BoardMetrics& metrics) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        for (auto square : Squares::all()) {
            ImU32 color = square.light() ? IM_COL32(240, 217, 181, 255) : IM_COL32(181, 136, 99, 255);
            Rect  rect  = square.normalized_rect().absolute(metrics.size, metrics.size);

            ImVec2 begin(metrics.origin.x + rect.left(), metrics.origin.y + rect.top());
            ImVec2 end(metrics.origin.x + rect.right(), metrics.origin.y + rect.bottom());

            drawList->AddRectFilled(begin, end, color);
        }
    }

    void highlights(const BoardMetrics& metrics) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        auto selectedSquare = m_context.state.selectedSquare;
        if (selectedSquare.has_value()) {
            const float THICKNESS = 0.05f * metrics.squareSize;
            Rect        rect      = selectedSquare.normalized_rect().absolute(metrics.size, metrics.size);

            ImVec2 begin(metrics.origin.x + rect.left() + THICKNESS * 0.3f,
                         metrics.origin.y + rect.top() + THICKNESS * 0.3f);
            ImVec2 end(metrics.origin.x + rect.right() - THICKNESS * 0.3f,
                       metrics.origin.y + rect.bottom() - THICKNESS * 0.3f);

            drawList->AddRect(begin, end, IM_WHITE, 0.0f, 0, THICKNESS);

            for (const auto& move : m_context.engine.moves(selectedSquare)) {
                Rect rect = move.to().normalized_rect().absolute(metrics.size, metrics.size);

                ImVec2 begin(metrics.origin.x + rect.left(), metrics.origin.y + rect.top());
                ImVec2 end(metrics.origin.x + rect.right(), metrics.origin.y + rect.bottom());

                ImVec2 center((begin.x + end.x) * 0.5f, (begin.y + end.y) * 0.5f);

                if (m_context.engine.at(move.to()) == Pieces::NONE) {
                    float radius = 0.08f * (end.x - begin.x);
                    drawList->AddCircleFilled(center, radius, IM_WHITE, 32);
                } else {
                    float radius = 0.5f * (end.x - begin.x) - THICKNESS * 0.3f;
                    drawList->AddCircle(center, radius, IM_REDDISH, 32, THICKNESS);
                }
            }
        }
    }

    void labels(const BoardMetrics& metrics) {
        ImDrawList*  drawList  = ImGui::GetWindowDrawList();
        const ImVec2 CHAR_SIZE = ImGui::CalcTextSize("A");

        for (auto file : Files::all()) {
            std::string label(1, static_cast<char>(a_CHAR_INDEX + file.value()));
            Square      ref(file, Ranks::R1);
            Rect        rect = ref.normalized_rect().absolute(metrics.size, metrics.size);

            float x = metrics.origin.x + rect.left() + rect.width / 2 - CHAR_SIZE.x / 2;
            drawList->AddText(ImVec2(x, metrics.origin.y - CHAR_SIZE.y - PIECE_MARGIN), IM_WHITE, label.c_str());
            drawList->AddText(ImVec2(x, metrics.origin.y + metrics.size + PIECE_MARGIN), IM_WHITE, label.c_str());
        }

        for (auto rank : Ranks::all()) {
            std::string label = std::to_string(rank.value() + 1);
            Square      ref(Files::FA, rank);
            Rect        rect = ref.normalized_rect().absolute(metrics.size, metrics.size);

            float y = metrics.origin.y + rect.top() + rect.height / 2 - CHAR_SIZE.y / 2;
            drawList->AddText(ImVec2(metrics.origin.x - CHAR_SIZE.x - PIECE_MARGIN, y), IM_WHITE, label.c_str());
            drawList->AddText(ImVec2(metrics.origin.x + metrics.size + PIECE_MARGIN, y), IM_WHITE, label.c_str());
        }
    }

    void player_names(const Rect<float>& panel) {
        ImDrawList* drawList  = ImGui::GetWindowDrawList();
        int         FONT_SIZE = ImGui::GetIO().Fonts->Fonts[0]->FontSize;

        drawList->AddText(ImVec2(panel.x, panel.y), IM_WHITE, "Player White");
        drawList->AddText(ImVec2(panel.x, panel.bottom() - static_cast<float>(FONT_SIZE)), IM_WHITE, "Player Black");
    }

    void pieces(const BoardMetrics& metrics) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        for (Square square : Squares::all()) {
            Rect rect = square.normalized_rect().absolute(metrics.size, metrics.size);

            ImVec2 begin(metrics.origin.x + rect.left() + PIECE_MARGIN, metrics.origin.y + rect.top() + PIECE_MARGIN);
            ImVec2 end(metrics.origin.x + rect.right() - PIECE_MARGIN, metrics.origin.y + rect.bottom() - PIECE_MARGIN);

            Piece piece = m_context.engine.board().at(square.value());
            if (piece.has_value()) {
                drawList->AddImage(m_context.textureManager.get(piece), begin, end);
            }
        }
    }
};
