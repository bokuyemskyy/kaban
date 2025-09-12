#include <imgui.h>

#include "gui_component.hpp"
#include "gui_context.hpp"
#include "window_registry.hpp"

class DemoWindow : IGuiComponent {
   public:
    using IGuiComponent::IGuiComponent;
    void draw() override {
        ImGui::ShowDemoWindow(&m_context.state.showDemoWindow);
        m_context.windowRegistry.add("Dear ImGui Demo");
    }
};