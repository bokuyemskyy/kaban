#include "glfw_wrapper.hpp"

#include <GL/gl.h>

#include <stdexcept>

GlfwWrapper::GlfwWrapper(int width, int height, const std::string& title, bool use_vsync) {
    if (glfwInit() != GL_TRUE) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    m_monitor = glfwGetPrimaryMonitor();
    m_window  = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

    if (!m_window) {
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowSizeLimits(m_window, width, height, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(use_vsync ? 1 : 0);

    glEnable(GL_BLEND);
}

GlfwWrapper::~GlfwWrapper() noexcept {
    if (m_window) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

Dimensions<int> GlfwWrapper::dimensions() const {
    Dimensions<int> dimensions{};
    glfwGetFramebufferSize(m_window, &dimensions.width, &dimensions.height);
    return dimensions;
}

void GlfwWrapper::finishFrame() const {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GlfwWrapper::fillFrame(float r, float g, float b, float a) const {
    const auto dims = dimensions();
    glViewport(0, 0, dims.width, dims.height);
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

bool GlfwWrapper::shouldClose() const noexcept { return glfwWindowShouldClose(m_window) != 0; }

void GlfwWrapper::setShouldClose(bool value) noexcept {
    glfwSetWindowShouldClose(m_window, value ? GLFW_TRUE : GLFW_FALSE);
}
