#include <imgui.h>

#include "board_view.hpp"
#include "captured_pieces.hpp"
#include "game_info.hpp"
#include "gui_component.hpp"
#include "gui_context.hpp"

constexpr float ITEM_SPACING         = 6;
constexpr float LEFT_PANEL_RATIO     = 0.6;
constexpr float VERTICAL_BOARD_RATIO = 0.86;

class GameView : IGuiComponent {
   public:
    using IGuiComponent::IGuiComponent;

    void draw() override {
        ImGui::GetStyle().ItemSpacing.y = ITEM_SPACING;
        ImGui::GetStyle().ItemSpacing.x = ITEM_SPACING;

        ImGui::SetNextWindowPos(ImVec2(0, ImGui::GetFrameHeight()));
        ImGui::SetNextWindowSize(ImVec2(m_ctx.io.DisplaySize.x, m_ctx.io.DisplaySize.y - ImGui::GetFrameHeight()));

        ImGui::Begin("GameView", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImVec2 available_space  = ImGui::GetContentRegionAvail();
        float  left_panel_width = available_space.x * LEFT_PANEL_RATIO;
        float  board_height     = available_space.y * VERTICAL_BOARD_RATIO;

        ImGui::BeginChild("LeftPanel", ImVec2(left_panel_width, available_space.y), 0);
        {
            ImGui::BeginChild("BoardView", ImVec2(0, board_height), 1);
            m_board_view.draw();
            ImGui::EndChild();

            ImGui::BeginChild("LostPieces", ImVec2(0, 0), 1);
            m_captured_pieces.draw();
            ImGui::EndChild();
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("RightPanel", ImVec2(0, available_space.y), 1);
        {
            m_game_info.draw();
        }
        ImGui::EndChild();

        ImGui::End();
    }

   private:
    BoardView      m_board_view{m_ctx};
    GameInfo       m_game_info{m_ctx};
    CapturedPieces m_captured_pieces{m_ctx};
};
