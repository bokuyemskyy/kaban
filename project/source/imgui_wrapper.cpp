#include <imgui_internal.h>

#include <algorithm>
#include <imgui_wrapper.hpp>
#include <string>

bool IMGUIWrapper::init(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");
    return true;
}

void IMGUIWrapper::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void IMGUIWrapper::showDemoWindow() { ImGui::ShowDemoWindow(); }

void IMGUIWrapper::setWindowPos(std::string title, float x, float y) {
    ImGui::SetWindowPos(title.c_str(), ImVec2(x, y));
}

void IMGUIWrapper::newFrame() {
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
    m_display_w = io.DisplaySize.x;
    m_display_h = io.DisplaySize.y;
}

void IMGUIWrapper::keepWindowInBounds(const char *windowName) {
    ImGuiWindow *window = ImGui::FindWindowByName(windowName);

    float maxX = m_display_w - window->Size.x;
    float maxY = m_display_h - window->Size.y;

    float newX = maxX > 0 ? std::clamp(window->Pos.x, 0.0f, maxX) : 0;
    float newY = maxY > 0 ? std::clamp(window->Pos.y, 0.0f, maxY) : 0;
    float newW = maxX > 0 ? window->Size.x : m_display_w;
    float newH = maxY > 0 ? window->Size.y : m_display_h;

    if (newX != window->Pos.x || newY != window->Pos.y || newW != window->Size.x ||
        newH != window->Size.y) {
        ImGui::SetWindowPos(windowName, ImVec2(newX, newY));
        ImGui::SetWindowSize(windowName, ImVec2(newW, newH));
    }
}