#include <imgui.h>

#include <string>

#include "gui_component.hpp"
#include "gui_context.hpp"

class GameInfo : IGuiComponent {
   public:
    using IGuiComponent::IGuiComponent;

    void draw() override {
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
};