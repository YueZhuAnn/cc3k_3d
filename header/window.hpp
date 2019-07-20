#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <imgui.h>

class Window{
protected:
    GLFWwindow          *m_window = nullptr;
    const unsigned      m_width;
    const unsigned      m_height;
    const unsigned      m_frame;
    unsigned            m_speed = 10;
    int                 m_framebufferWidth = 0;
    int                 m_framebufferHeight = 0;
    const std::string   m_title;  
    bool                m_paused = false;
    int                 shaderProgram;
    void windowCreation();
    void imguiInit();
    void imguiDestroy();
    virtual void init();
    virtual void processInput();
    virtual void guiLogic();
    virtual void appLogic();
    virtual void draw();
    virtual void setUp();
    void setSpeed(unsigned speed);
public:
    Window(unsigned w=800, unsigned h=600, unsigned f=24, std::string title = "window");
    ~Window();
    void run();
};