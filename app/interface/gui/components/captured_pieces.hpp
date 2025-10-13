#include <imgui.h>

#include "gui_component.hpp"
#include "gui_context.hpp"

class CapturedPieces : IGuiComponent {
   public:
    using IGuiComponent::IGuiComponent;
    void draw() override {
        ImGui::Text("Captured Pieces (0)");
        ImGui::Text("White: ");
        ImGui::Text("Black: ");
    }
};