#pragma once

#include <GLFW/glfw3.h>

#include <string>

#include "dimensions.hpp"

class GlfwWrapper {
   public:
    GlfwWrapper(int width, int height, const std::string& title, bool use_vsync);
    ~GlfwWrapper() noexcept;

    GlfwWrapper(const GlfwWrapper&)                = delete;
    GlfwWrapper& operator=(const GlfwWrapper&)     = delete;
    GlfwWrapper(GlfwWrapper&&) noexcept            = delete;
    GlfwWrapper& operator=(GlfwWrapper&&) noexcept = delete;

    void fill_frame(float r = 0, float g = 0, float b = 0, float a = 1) const;
    void finish_frame() const;

    [[nodiscard]] bool should_close() const noexcept;
    void               set_should_close(bool value) noexcept;

    [[nodiscard]] GLFWwindow*     window() const noexcept { return m_window; }
    [[nodiscard]] static double   time() noexcept { return glfwGetTime(); }
    [[nodiscard]] Dimensions<int> dimensions() const;

   private:
    GLFWwindow*  m_window{nullptr};
    GLFWmonitor* m_monitor{nullptr};
};
