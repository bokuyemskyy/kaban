#pragma once

#include <GLFW/glfw3.h>

#include <string>

#include "dimensions.hpp"

class GLFWWrapper {
   public:
    GLFWWrapper(int width, int height, const std::string& title, bool use_vsync);
    ~GLFWWrapper() noexcept;

    GLFWWrapper(const GLFWWrapper&)                = delete;
    GLFWWrapper& operator=(const GLFWWrapper&)     = delete;
    GLFWWrapper(GLFWWrapper&&) noexcept            = delete;
    GLFWWrapper& operator=(GLFWWrapper&&) noexcept = delete;

    void beginFrame();
    void fillFrame(float r, float g, float b, float a) const;
    void finishFrame() const;

    [[nodiscard]] bool windowShouldClose() const noexcept;
    void               setWindowShouldClose(bool value) noexcept;

    [[nodiscard]] GLFWwindow*     window() const noexcept { return m_window; }
    [[nodiscard]] static double   time() noexcept { return glfwGetTime(); }
    [[nodiscard]] Dimensions<int> dimensions() const;

   private:
    GLFWwindow*  m_window{nullptr};
    GLFWmonitor* m_monitor{nullptr};
};
