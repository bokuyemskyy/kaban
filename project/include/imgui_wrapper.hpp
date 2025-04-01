#ifndef IMGUI_WRAPPER_HPP
#define IMGUI_WRAPPER_HPP

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <string>

class GLFWwindow {};

class IMGUIWrapper {
   public:
    static bool init(GLFWwindow *window);
    static void shutdown();
    static void showDemoWindow();
    void newFrame();
    void finishFrame();
    void setWindowPos(std::string title, float x, float y);
    void updateDimensions();
    void keepWindowInBounds(const char *windowName);

   private:
    float m_display_w, m_display_h;
};

#endif