#include <iostream>
#include <chrono> 
#include <thread>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.hpp"

using namespace std;

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

Window::Window(unsigned w, unsigned h, unsigned f, string title):
    m_width(w), m_height(h), m_frame(f), m_title(title){
}

Window::~Window(){
    glfwTerminate();
}

void Window::windowCreation(){
    if (glfwInit() == GL_FALSE) {
        cerr << "window init failed" << endl;
        abort();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // create window
    m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
    if (m_window == nullptr) {
        glfwTerminate();
        cerr << "Call to glfwCreateWindow failed" << endl;
        abort();
    }
    glfwMakeContextCurrent(m_window);
    glfwGetFramebufferSize(m_window, &m_framebufferWidth, &m_framebufferHeight);
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        cerr <<  "Failed to initialize GLAD" << endl;
        abort();
    }

    // Set up imgui
    const char* glsl_version = "#version 150";
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
}

// virtual init function
void Window::init(){}

// virtual process input function
void Window::processInput(){}

// virtual guiLogic function
void Window::guiLogic(){}

// virtual appLogic function
void Window::appLogic(){}

// virtual draw function
void Window::draw(){}

// virtual set up how to draw
void Window::setUp(){}

void Window::setSpeed(unsigned speed){
    m_speed = speed;
}

void Window::imguiInit(){
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

}

void Window::imguiDestroy(){
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
void Window::run(){
    auto m_time = chrono::high_resolution_clock::now();
    auto curtime = chrono::high_resolution_clock::now();
    while (!glfwWindowShouldClose(m_window))
    {
        unsigned frametime = 1000/m_speed;
        glfwPollEvents();
        imguiInit();
        setUp();
        if (!m_paused) {
            processInput();
            
            // Apply application-specific logic
	        appLogic();
        }
	    guiLogic();
        draw();

        ImGui::Render();
	    // In case of a window resize, get new framebuffer dimensions.
        glfwGetFramebufferSize(m_window, &m_framebufferWidth,
            &m_framebufferHeight);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		// Finally, blast everything to the screen.
        glfwSwapBuffers(m_window);

        curtime = chrono::high_resolution_clock::now();
        auto t_interval = chrono::duration_cast<chrono::milliseconds>(curtime  - m_time);
        int interval = frametime - t_interval.count();
        if(interval > 0){
            this_thread::sleep_for(chrono::milliseconds(interval));
        }
        m_time = chrono::high_resolution_clock::now();
    }
    imguiDestroy();
    glfwDestroyWindow(m_window);
}
