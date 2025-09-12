#include <imgui.h>

#include "gui_component.hpp"
#include "gui_context.hpp"
#include "gui_state.hpp"

class MainMenu : IGuiComponent {
   public:
    using IGuiComponent::IGuiComponent;

    void draw() override {
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("New Game")) {
                }
                if (ImGui::MenuItem("Open Game")) {
                }
                if (ImGui::MenuItem("Save Game")) {
                }
                if (ImGui::MenuItem("Exit")) {
                    m_context.state.shouldClose = true;
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
            if (ImGui::BeginMenu("Options")) {
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help")) {
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
};