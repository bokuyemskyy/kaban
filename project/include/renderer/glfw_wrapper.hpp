#pragma once

#include <GLFW/glfw3.h>

#include <string>

#include "utils.hpp"

class GLFWWrapper {
   public:
    GLFWWrapper()  = default;
    ~GLFWWrapper() = default;

    void initialize(Dimensions dimensions, const char *title, bool useVsync);
    void terminate();

    void updateDimensions();
    void beginFrame();
    void fillFrame(float r, float g, float b, float a) const;
    void finishFrame() const;

    bool windowShouldClose() { return glfwWindowShouldClose(m_window) != 0; };
    void setWindowShouldClose(bool value);

    [[nodiscard]] Dimensions    dimensions() const noexcept { return m_dimensions; }
    [[nodiscard]] GLFWwindow   *window() const noexcept { return m_window; }
    [[nodiscard]] static double time() noexcept { return glfwGetTime(); }
    [[nodiscard]] bool          shouldClose() { return glfwWindowShouldClose(m_window) != 0; }

   private:
    Dimensions  m_dimensions{};
    std::string m_title;
    bool        m_useVsync{};

    GLFWmonitor *m_monitor{};
    GLFWwindow  *m_window{};

    bool m_initialized{};
};
