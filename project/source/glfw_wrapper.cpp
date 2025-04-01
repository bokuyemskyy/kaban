#include "glfw_wrapper.hpp"

#include <GL/gl.h>
#include <imgui.h>
#include <imgui_internal.h>

#include <cstring>
#include <error_handler.hpp>

bool GLFWWrapper::init(float width, float height, const char *title, bool useVsync) {
    glfwSetErrorCallback(errorCallback);

    if (glfwInit() != GL_TRUE) {
        handleError(0, "Failed to initialize GLFW");
        shutdown();
        return false;
    }
    /*
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        */
    m_monitor = glfwGetPrimaryMonitor();
    m_mode = glfwGetVideoMode(m_monitor);

    handleError(0, title);
    m_window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), "My Window",
                                NULL, NULL);

    if (m_window == nullptr) {
        handleError(0, "Failed to initialize a window");
        shutdown();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(useVsync ? 1 : 0);

    updateDimensions();

    return true;
}

void GLFWWrapper::shutdown() {
    if (m_window != nullptr) {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}

bool GLFWWrapper::windowShouldClose() { return glfwWindowShouldClose(m_window) != 0; }

float GLFWWrapper::getTime() { return static_cast<float>(glfwGetTime()); }

void GLFWWrapper::updateDimensions() {
    glfwGetFramebufferSize(getWindow(), &m_display_w, &m_display_h);
}

void GLFWWrapper::newFrame() { updateDimensions(); }

void GLFWWrapper::finishFrame() {
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}

void GLFWWrapper::fillFrame(float r, float g, float b, float a) const {
    glViewport(0, 0, GLsizei(m_display_w), GLsizei(m_display_h));
    glClearColor(GLclampf(r), GLclampf(g), GLclampf(b), GLclampf(a));
    glClear(GL_COLOR_BUFFER_BIT);
}

GLFWwindow *GLFWWrapper::getWindow() const { return m_window; }

void GLFWWrapper::setWindowShouldClose(bool value) {
    glfwSetWindowShouldClose(m_window, static_cast<int>(value));
}