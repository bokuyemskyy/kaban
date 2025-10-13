#include <imgui.h>

#include "gui_component.hpp"
#include "gui_context.hpp"
#include "window_registry.hpp"

class DemoWindow : IGuiComponent {
   public:
    using IGuiComponent::IGuiComponent;
    void draw() override {
        ImGui::ShowDemoWindow(&m_ctx.state.show_demo_window);
        m_ctx.windowRegistry.add("Dear ImGui Demo");
    }
};