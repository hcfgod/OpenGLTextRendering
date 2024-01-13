#include "Window.h"

namespace Echo
{
    Window::Window()
    {
        m_Window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);

        if (!m_Window)
        {
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(m_Window);
    }

    Window::~Window()
    {
        glfwTerminate();
    }

    bool Window::ShouldWindowClose()
    {
        return glfwWindowShouldClose(m_Window);
    }

    void Window::SetShouldWindowClose(bool value)
    {
        glfwSetWindowShouldClose(m_Window, value ? 1 : 0);
    }

    glm::vec2 Window::GetWindowSize()
    {
        int width, height;
        glfwGetWindowSize(m_Window, &width, &height);

        return glm::vec2(width, height);
    }

    GLFWwindow* Window::GetGlfwWindow()
    {
        return m_Window;
    }
}