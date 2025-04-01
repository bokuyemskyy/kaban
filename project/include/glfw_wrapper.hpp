#ifndef GLFW_WRAPPER_HPP
#define GLFW_WRAPPER_HPP

#include <GLFW/glfw3.h>

class GLFWWrapper {
   public:
    GLFWWrapper() = default;
    ~GLFWWrapper() = default;
    bool init(float width, float height, const char *title, bool useVsync = false);
    void shutdown();
    bool windowShouldClose();
    void setWindowShouldClose(bool value);
    void updateDimensions();
    static float getTime();
    void newFrame();
    void finishFrame();
    void fillFrame(float r, float g, float b, float a) const;
    [[nodiscard]] int getWidth() const { return m_display_w; };
    [[nodiscard]] int getHeight() const { return m_display_h; };
    GLFWwindow *getWindow() const;

   private:
    GLFWwindow *m_window;
    GLFWmonitor *m_monitor;
    const GLFWvidmode *m_mode;

    int m_display_w, m_display_h;
};

#endif