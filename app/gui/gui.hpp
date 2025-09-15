#pragma once

#include <string>

#include "demo_window.hpp"
#include "engine.hpp"
#include "game_view.hpp"
#include "glfw_wrapper.hpp"
#include "gui_state.hpp"
#include "imgui_wrapper.hpp"
#include "main_menu.hpp"

class Gui {
   public:
    Gui(Engine& engine, int width, int height, const std::string& title, bool use_vsync)
        : m_glfw(width, height, title, use_vsync),
          m_imgui(m_glfw.window()),
          m_context{.state          = m_state,
                    .io             = m_imgui.io(),
                    .engine         = engine,
                    .textureManager = m_texture_manager,
                    .windowRegistry = m_window_registry},
          m_main_menu(m_context),
          m_demo_window(m_context),
          m_game_view(m_context) {
        updateTime();
    }
    void run() {
        while (!m_context.state.should_close && !m_glfw.shouldClose()) {
            updateTime();

            m_imgui.beginFrame();
            m_glfw.fillFrame();

            m_window_registry.clear();

            m_main_menu.draw();
            m_game_view.draw();
            if (m_context.state.show_demo_window) m_demo_window.draw();

            for (const auto& name : m_window_registry.get()) m_imgui.keepWindowInBounds(name.c_str());

            m_imgui.finishFrame();
            m_glfw.finishFrame();
        }
    }

   private:
    void updateTime() {
        m_current_time = GlfwWrapper::time();
        m_delta_time   = m_current_time - m_last_time;
        m_last_time    = m_current_time;
    };

    GlfwWrapper  m_glfw;
    ImGuiWrapper m_imgui;

    GuiState       m_state;
    TextureManager m_texture_manager;
    WindowRegistry m_window_registry;

    GuiContext m_context;

    MainMenu   m_main_menu;
    DemoWindow m_demo_window;
    GameView   m_game_view;

    double m_last_time    = 0;
    double m_delta_time   = 0;
    double m_current_time = 0;
};