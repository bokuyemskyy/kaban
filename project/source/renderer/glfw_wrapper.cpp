#include "glfw_wrapper.hpp"

#include <GL/gl.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <stdexcept>

void GLFWWrapper::initialize(int width, int height, const char *title, bool use_vsync) {
    m_width     = width;
    m_height    = height;
    m_title     = title;
    m_use_vsync = use_vsync;

    if (glfwInit() != GL_TRUE) {
        throw std::runtime_error("Failed to initialize GLFW");
        terminate();
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    m_monitor = glfwGetPrimaryMonitor();
    m_window  = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);

    glfwSetWindowSizeLimits(m_window, m_width, m_height, GLFW_DONT_CARE, GLFW_DONT_CARE);

    if (m_window == nullptr) {
        throw std::runtime_error("Failed to initialize a window");
        terminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(use_vsync ? 1 : 0);

    updateDimensions();

    m_initialized = true;
}

void GLFWWrapper::terminate() {
    if (m_window != nullptr) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

void GLFWWrapper::updateDimensions() { glfwGetFramebufferSize(window(), &m_width, &m_height); }

void GLFWWrapper::beginFrame() { updateDimensions(); }

void GLFWWrapper::finishFrame() const {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GLFWWrapper::fillFrame(float r, float g, float b, float a) const {
    glViewport(0, 0, GLsizei(m_width), GLsizei(m_height));
    glClearColor(GLclampf(r), GLclampf(g), GLclampf(b), GLclampf(a));
    glClear(GL_COLOR_BUFFER_BIT);
}
