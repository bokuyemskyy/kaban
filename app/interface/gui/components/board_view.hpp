#pragma once

#include <imgui.h>

#include <algorithm>
#include <cmath>

#include "engine.hpp"
#include "gui_component.hpp"
#include "gui_context.hpp"
#include "move.hpp"
#include "piece.hpp"
#include "rect.hpp"
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
        handleInput();

        Rect<float>  panel   = calculatePanel();
        BoardMetrics metrics = boardMetrics(panel);

        squares(metrics);
        highlights(metrics);
        labels(metrics);
        playerNames(panel);
        pieces(metrics);
    }

   private:
    struct BoardMetrics {
        ImVec2 origin;
        float  size;
        float  squareSize;
    };

    Rect<float> calculatePanel() {
        ImVec2 panelMin  = ImGui::GetCursorScreenPos();
        ImVec2 availSize = ImGui::GetContentRegionAvail();
        return Rect(panelMin.x, panelMin.y, availSize.x, availSize.y);
    }

    BoardMetrics boardMetrics(const Rect<float>& panel) {
        const float WINDOW_PADDING = ImGui::GetStyle().WindowPadding.x;
        const float FONT_SIZE      = ImGui::GetIO().Fonts->Fonts[0]->FontSize;
        ImVec2    availSize(panel.width, panel.height);

        float  boardSize = std::min(availSize.x, availSize.y) - 2.0f * (FONT_SIZE + WINDOW_PADDING + BOARD_MARGIN);
        ImVec2 origin(panel.x + (availSize.x - boardSize) / 2, panel.y + (availSize.y - boardSize) / 2);

        return BoardMetrics{.origin = origin, .size = boardSize, .squareSize = boardSize / 8.0f};
    }

    void handleInput() {
        if (!ImGui::IsWindowHovered()) return;

        Rect<float>  panel   = calculatePanel();
        BoardMetrics metrics = boardMetrics(panel);

        Square& selected = m_ctx.state.selected_square;

        if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
            m_ctx.engine.unmakeMove();
            selected = Squares::NONE;
            return;
        }

        ImGuiIO& io = m_ctx.io;

        if (ImGui::IsMouseClicked(0)) {
            ImVec2 mouse = io.MousePos;

            float rel_x = mouse.x - metrics.origin.x;
            float rel_y = mouse.y - metrics.origin.y;

            File file = File(static_cast<uint8_t>(std::floor(rel_x / metrics.squareSize)));
            Rank rank = Rank(static_cast<uint8_t>(7 - std::floor(rel_y / metrics.squareSize)));

            if (!file.hasValue() || !rank.hasValue()) {
                selected = Squares::NONE;
                return;
            }

            Square clicked(file, rank);

            if (selected != Squares::NONE) {
                auto moves = m_ctx.engine.moves();
                auto it    = std::ranges::find_if(
                    moves, [&](const Move& m) { return m.from() == selected && m.to() == clicked; });
                if (it != moves.end()) {
                    m_ctx.engine.makeMove(*it);
                    selected = Squares::NONE;
                } else if (m_ctx.engine.at(clicked).hasValue()) {
                    selected = clicked;
                } else {
                    selected = Squares::NONE;
                }
            } else {
                if (m_ctx.engine.at(clicked).hasValue()) {
                    selected = clicked;
                }
            }
        }
    }

    void squares(const BoardMetrics& metrics) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        for (auto square : Squares::all()) {
            ImU32 color = square.light() ? IM_COL32(240, 217, 181, 255) : IM_COL32(181, 136, 99, 255);
            Rect  rect  = getNormalizedRect(square).absolute(metrics.size, metrics.size);

            ImVec2 begin(metrics.origin.x + rect.left(), metrics.origin.y + rect.top());
            ImVec2 end(metrics.origin.x + rect.right(), metrics.origin.y + rect.bottom());

            drawList->AddRectFilled(begin, end, color);
        }
    }

    void highlights(const BoardMetrics& metrics) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        auto selected = m_ctx.state.selected_square;
        if (selected != Squares::NONE) {
            const float THICKNESS = 0.05f * metrics.squareSize;
            Rect        rect      = getNormalizedRect(selected).absolute(metrics.size, metrics.size);

            ImVec2 begin(metrics.origin.x + rect.left() + THICKNESS * 0.25f,
                         metrics.origin.y + rect.top() + THICKNESS * 0.25f);
            ImVec2 end(metrics.origin.x + rect.right() - THICKNESS * 0.25f,
                       metrics.origin.y + rect.bottom() - THICKNESS * 0.25f);

            drawList->AddRect(begin, end, IM_WHITE, 0.0f, 0, THICKNESS);

            for (const auto& move : m_ctx.engine.moves()) {
                if (move.from() != selected) continue;

                Rect move_rect = getNormalizedRect(move.to()).absolute(metrics.size, metrics.size);

                ImVec2 move_begin(metrics.origin.x + move_rect.left(), metrics.origin.y + move_rect.top());
                ImVec2 move_end(metrics.origin.x + move_rect.right(), metrics.origin.y + move_rect.bottom());

                ImVec2 center((move_begin.x + move_end.x) * 0.5f, (move_begin.y + move_end.y) * 0.5f);

                if (m_ctx.engine.at(move.to()) == Pieces::NONE) {
                    float radius = 0.08f * (move_end.x - move_begin.x);
                    drawList->AddCircleFilled(center, radius, IM_WHITE, 32);
                } else {
                    float radius = 0.5f * (move_end.x - move_begin.x) - THICKNESS * 0.25f;
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
            Rect        rect = getNormalizedRect(ref).absolute(metrics.size, metrics.size);

            float x = metrics.origin.x + rect.left() + rect.width / 2 - CHAR_SIZE.x / 2;
            drawList->AddText(ImVec2(x, metrics.origin.y - CHAR_SIZE.y - PIECE_MARGIN), IM_WHITE, label.c_str());
            drawList->AddText(ImVec2(x, metrics.origin.y + metrics.size + PIECE_MARGIN), IM_WHITE, label.c_str());
        }

        for (auto rank : Ranks::all()) {
            std::string label = std::to_string(rank.value() + 1);
            Square      ref(Files::FA, rank);
            Rect        rect = getNormalizedRect(ref).absolute(metrics.size, metrics.size);

            float y = metrics.origin.y + rect.top() + rect.height / 2 - CHAR_SIZE.y / 2;
            drawList->AddText(ImVec2(metrics.origin.x - CHAR_SIZE.x - PIECE_MARGIN, y), IM_WHITE, label.c_str());
            drawList->AddText(ImVec2(metrics.origin.x + metrics.size + PIECE_MARGIN, y), IM_WHITE, label.c_str());
        }
    }

    void playerNames(const Rect<float>& panel) {
        ImDrawList* drawList  = ImGui::GetWindowDrawList();
        float         FONT_SIZE = ImGui::GetIO().Fonts->Fonts[0]->FontSize;

        drawList->AddText(ImVec2(panel.x, panel.y), IM_WHITE, "Player White");
        drawList->AddText(ImVec2(panel.x, panel.bottom() - FONT_SIZE), IM_WHITE, "Player Black");
    }

    void pieces(const BoardMetrics& metrics) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        for (Square square : Squares::all()) {
            Rect rect = getNormalizedRect(square).absolute(metrics.size, metrics.size);

            ImVec2 begin(metrics.origin.x + rect.left() + PIECE_MARGIN, metrics.origin.y + rect.top() + PIECE_MARGIN);
            ImVec2 end(metrics.origin.x + rect.right() - PIECE_MARGIN, metrics.origin.y + rect.bottom() - PIECE_MARGIN);

            Piece piece = m_ctx.engine.board().at(square.value());
            if (piece.hasValue()) {
                drawList->AddImage(m_ctx.textureManager.get(piece), begin, end);
            }
        }
    }

    NormalizedRect<float> getNormalizedRect(Square square) const {
        return {static_cast<float>(square.file().value()) / Files::count(),
                1 - (1.0f / Ranks::count()) - (static_cast<float>(square.rank().value()) / Ranks::count()),
                1.0f / Files::count(), 1.0f / Ranks::count()};
    }
};
