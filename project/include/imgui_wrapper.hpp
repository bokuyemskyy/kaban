#ifndef IMGUI_WRAPPER_HPP
#define IMGUI_WRAPPER_HPP

#define IMGUI_ENABLE_DOCKING

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <string>

#include "utils.hpp"

class GLFWwindow {};

class IMGUIWrapper {
   public:
    IMGUIWrapper() = default;
    ~IMGUIWrapper() = default;

    void initialize(GLFWwindow *window);
    void terminate();

    void updateDimensions();
    void showDemoWindow();

    void beginFrame();
    void finishFrame();

    void setWindowPos(const std::string &title, float x, float y);
    void keepWindowInBounds(const char *windowName) const;

   private:
    Dimensions m_dimensions;
};

#endif