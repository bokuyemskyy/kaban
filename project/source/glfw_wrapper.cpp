#include "glfw_wrapper.hpp"

#include <GL/gl.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <stdexcept>

#include "utils.hpp"

void GLFWWrapper::initialize(Dimensions dimensions, const char *title, bool useVsync) {
    m_dimensions = dimensions;
    m_title      = title;
    m_useVsync   = useVsync;

    if (glfwInit() != GL_TRUE) {
        throw std::runtime_error("Failed to initialize GLFW");
        terminate();
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    m_monitor = glfwGetPrimaryMonitor();
    m_window  = glfwCreateWindow(m_dimensions.width, m_dimensions.height, m_title.c_str(), nullptr, nullptr);

    glfwSetWindowSizeLimits(m_window, dimensions.width, dimensions.height, GLFW_DONT_CARE, GLFW_DONT_CARE);

    if (m_window == nullptr) {
        throw std::runtime_error("Failed to initialize a window");
        terminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(useVsync ? 1 : 0);

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

void GLFWWrapper::updateDimensions() { glfwGetFramebufferSize(window(), &m_dimensions.width, &m_dimensions.height); }

void GLFWWrapper::beginFrame() { updateDimensions(); }

void GLFWWrapper::finishFrame() const {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GLFWWrapper::fillFrame(float r, float g, float b, float a) const {
    glViewport(0, 0, GLsizei(m_dimensions.width), GLsizei(m_dimensions.height));
    glClearColor(GLclampf(r), GLclampf(g), GLclampf(b), GLclampf(a));
    glClear(GL_COLOR_BUFFER_BIT);
}

void GLFWWrapper::setWindowShouldClose(bool value) { glfwSetWindowShouldClose(m_window, static_cast<int>(value)); }