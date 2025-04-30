#include "imgui_wrapper.hpp"

#include <imgui.h>
#include <imgui_internal.h>

#include <algorithm>
#include <string>

void IMGUIWrapper::initialize(GLFWwindow *window) {
    IMGUI_CHECKVERSION();

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    updateDimensions();
}

void IMGUIWrapper::terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void IMGUIWrapper::showDemoWindow() { ImGui::ShowDemoWindow(); }

void IMGUIWrapper::setWindowPos(const std::string &title, float x, float y) {
    ImGui::SetWindowPos(title.c_str(), ImVec2(x, y));
}

void IMGUIWrapper::beginFrame() {
    updateDimensions();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void IMGUIWrapper::finishFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void IMGUIWrapper::updateDimensions() {
    ImGuiIO &io = ImGui::GetIO();
    m_dimensions.width = static_cast<int>(io.DisplaySize.x);
    m_dimensions.height = static_cast<int>(io.DisplaySize.y);
}

void IMGUIWrapper::keepWindowInBounds(const char *windowName) const {
    ImGuiWindow *window = ImGui::FindWindowByName(windowName);

    ImVec2 maxPosition{static_cast<float>(m_dimensions.width) - window->Size.x,
                       static_cast<float>(m_dimensions.height) - window->Size.y};

    ImVec2 newPosition{maxPosition.x > 0 ? std::clamp(window->Pos.x, 0.0F, maxPosition.x) : 0,
                       maxPosition.y > 0 ? std::clamp(window->Pos.y, 0.0F, maxPosition.y) : 0};
    ImVec2 newSize{maxPosition.x > 0 ? window->Size.x : static_cast<float>(m_dimensions.width),
                   maxPosition.y > 0 ? window->Size.y : static_cast<float>(m_dimensions.height)};

    if (newPosition.x != window->Pos.x || newPosition.y != window->Pos.y ||
        newSize.x != window->Size.x || newSize.y != window->Size.y) {
        ImGui::SetWindowPos(windowName, newPosition);
        ImGui::SetWindowSize(windowName, newSize);
    }
}