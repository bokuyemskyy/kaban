#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "dimensions.hpp"

struct GLFWwindow;

class ImGuiWrapper {
   public:
    struct Config {
        const char *glsl_version;
        bool        install_callbacks;
        bool        enable_docking;
        bool        enable_viewports;
        bool        dark_style;
        const char *ini_filename;

        Config() noexcept
            : glsl_version("#version 150"),
              install_callbacks(true),
              enable_docking(true),
              enable_viewports(false),
              dark_style(true),
              ini_filename(nullptr) {}
    };

    explicit ImGuiWrapper(GLFWwindow *window, const Config &cfg = Config{});
    ~ImGuiWrapper() noexcept;

    ImGuiWrapper(const ImGuiWrapper &)            = delete;
    ImGuiWrapper &operator=(const ImGuiWrapper &) = delete;
    ImGuiWrapper(ImGuiWrapper &&)                 = delete;
    ImGuiWrapper &operator=(ImGuiWrapper &&)      = delete;

    [[nodiscard]] Dimensions<int> dimensions() const;

    void beginFrame();
    void finishFrame();

    [[nodiscard]] ImGuiIO &io() const { return ImGui::GetIO(); }

    void keepWindowInBounds(const char *windowName) const;

   private:
    GLFWwindow *m_window{nullptr};
    Config      m_cfg{};
};