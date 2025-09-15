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
                    .textureManager = m_textureManager,
                    .windowRegistry = m_windowRegistry},
          m_mainMenu(m_context),
          m_demoWindow(m_context),
          m_gameView(m_context) {
        updateTime();
    }
    void run() {
        while (!m_context.state.shouldClose && !m_glfw.shouldClose()) {
            updateTime();

            m_imgui.beginFrame();
            m_glfw.fillFrame();

            m_windowRegistry.clear();

            m_mainMenu.draw();
            m_gameView.draw();
            if (m_context.state.showDemoWindow) m_demoWindow.draw();

            for (const auto& name : m_windowRegistry.get()) m_imgui.keepWindowInBounds(name.c_str());

            m_imgui.finishFrame();
            m_glfw.finishFrame();
        }
    }

   private:
    void updateTime() {
        m_currentTime = GlfwWrapper::time();
        m_deltaTime   = m_currentTime - m_lastTime;
        m_lastTime    = m_currentTime;
    };

    GlfwWrapper  m_glfw;
    ImGuiWrapper m_imgui;

    GuiState       m_state;
    TextureManager m_textureManager;
    WindowRegistry m_windowRegistry;

    GuiContext m_context;

    MainMenu   m_mainMenu;
    DemoWindow m_demoWindow;
    GameView   m_gameView;

    double m_lastTime    = 0;
    double m_deltaTime   = 0;
    double m_currentTime = 0;
};