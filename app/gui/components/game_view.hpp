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
        ImGui::SetNextWindowSize(
            ImVec2(m_context.io.DisplaySize.x, m_context.io.DisplaySize.y - ImGui::GetFrameHeight()));

        ImGui::Begin("GameView", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoBringToFrontOnFocus);

        ImVec2 availSpace     = ImGui::GetContentRegionAvail();
        float  leftPanelWidth = availSpace.x * LEFT_PANEL_RATIO;
        float  boardHeight    = availSpace.y * VERTICAL_BOARD_RATIO;

        ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, availSpace.y), 0);
        {
            ImGui::BeginChild("BoardView", ImVec2(0, boardHeight), 1);
            m_boardView.draw();
            ImGui::EndChild();

            ImGui::BeginChild("LostPieces", ImVec2(0, 0), 1);
            m_capturedPieces.draw();
            ImGui::EndChild();
        }
        ImGui::EndChild();

        ImGui::SameLine();
        ImGui::BeginChild("RightPanel", ImVec2(0, availSpace.y), 1);
        {
            m_gameInfo.draw();
        }
        ImGui::EndChild();

        ImGui::End();
    }

   private:
    BoardView      m_boardView{m_context};
    GameInfo       m_gameInfo{m_context};
    CapturedPieces m_capturedPieces{m_context};
};
