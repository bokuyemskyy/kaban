#pragma once

#include <GLFW/glfw3.h>

#include <string>

class GLFWWrapper {
   public:
    void initialize(int width, int height, const char *title, bool use_vsync);
    void terminate();

    void updateDimensions();
    void beginFrame();
    void fillFrame(float r, float g, float b, float a) const;
    void finishFrame() const;

    bool windowShouldClose() { return glfwWindowShouldClose(m_window) != 0; };
    void setWindowShouldClose(bool value) { glfwSetWindowShouldClose(m_window, static_cast<int>(value)); }

    [[nodiscard]] int           width() const noexcept { return m_width; }
    [[nodiscard]] int           height() const noexcept { return m_height; }
    [[nodiscard]] GLFWwindow   *window() const noexcept { return m_window; }
    [[nodiscard]] static double time() noexcept { return glfwGetTime(); }
    [[nodiscard]] bool          shouldClose() { return glfwWindowShouldClose(m_window) != 0; }

   private:
    int         m_width, m_height;
    bool        m_use_vsync;
    std::string m_title;

    GLFWmonitor *m_monitor;
    GLFWwindow  *m_window;

    bool m_initialized;
};
