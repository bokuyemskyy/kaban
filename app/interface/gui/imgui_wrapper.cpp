#include "imgui_wrapper.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <algorithm>
#include <stdexcept>

#include "dimensions.hpp"

ImGuiWrapper::ImGuiWrapper(GLFWwindow* window, const Config& cfg) {  // NOLINT
    if (!window) {
        throw std::invalid_argument("ImGuiWrapper::initialize: window is null");
    }

    m_window = window;
    m_cfg    = cfg;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& _io = ImGui::GetIO();

    // if (m_cfg.enable_docking) io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // if (m_cfg.enable_viewports) io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    _io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    _io.IniFilename = m_cfg.ini_filename;
    if (m_cfg.dark_style)
        ImGui::StyleColorsDark();
    else
        ImGui::StyleColorsLight();
    if (m_cfg.enable_viewports) {
        ImGuiStyle& style                 = ImGui::GetStyle();
        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    if (!ImGui_ImplGlfw_InitForOpenGL(m_window, m_cfg.install_callbacks)) {
        ImGui::DestroyContext();
        throw std::runtime_error("ImGui_ImplGlfw_InitForOpenGL failed");
    }
    if (!ImGui_ImplOpenGL3_Init(m_cfg.glsl_version)) {
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        throw std::runtime_error("ImGui_ImplOpenGL3_Init failed");
    }
}

ImGuiWrapper::~ImGuiWrapper() noexcept {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiWrapper::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiWrapper::finishFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

Dimensions<int> ImGuiWrapper::dimensions() {
    const ImGuiIO& _io = ImGui::GetIO();
    return Dimensions{static_cast<int>(_io.DisplaySize.x), static_cast<int>(_io.DisplaySize.y)};
}

void ImGuiWrapper::keepWindowInBounds(const char* windowName) {
    ImGuiWindow const* window = ImGui::FindWindowByName(windowName);

    auto         frame_dimensions = dimensions();
    const ImVec2 maxPosition{static_cast<float>(frame_dimensions.width) - window->Size.x,
                             static_cast<float>(frame_dimensions.height) - window->Size.y};

    const ImVec2 newPosition{maxPosition.x > 0 ? std::clamp(window->Pos.x, 0.0F, maxPosition.x) : 0,
                             maxPosition.y > 0 ? std::clamp(window->Pos.y, 0.0F, maxPosition.y) : 0};
    const ImVec2 newSize{maxPosition.x > 0 ? window->Size.x : static_cast<float>(frame_dimensions.width),
                         maxPosition.y > 0 ? window->Size.y : static_cast<float>(frame_dimensions.height)};

    if (newPosition.x != window->Pos.x || newPosition.y != window->Pos.y || newSize.x != window->Size.x ||
        newSize.y != window->Size.y) {
        ImGui::SetWindowPos(windowName, newPosition);
        ImGui::SetWindowSize(windowName, newSize);
    }
}