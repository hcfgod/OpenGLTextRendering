#include "Application.h"

#include <Shader.h>
#include <App/Window/Window.h>
#include <OrthographicCamera.h>

namespace Echo
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);

	Application::Application()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = new Window();
		glfwSetFramebufferSizeCallback(m_Window->GetGlfwWindow(), framebuffer_size_callback);

		glewInit();
	}

	Application::~Application()
	{
		delete m_Window;

        glfwTerminate();
	}

	void Application::Run()
	{
        Awake();

        while (!m_Window->ShouldWindowClose())
        {
            Render();
        }
	}

    void Application::Awake()
    {
        textRenderer.Init();
    }

	void Application::Render()
	{
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        textRenderer.RenderText("This is a sample text", "amiri-regular.ttf", 5, 55, 0.2f, glm::vec3(1));
        textRenderer.RenderText("This is a sample text", "Raleway-Italic.ttf", 5, 5, 0.5f, glm::vec3(1), 0.2f, 1.0f);

        glfwSwapBuffers(m_Window->GetGlfwWindow());
        glfwPollEvents();
	}

    void Application::OnResize(int width, int height)
    {
        glViewport(0, 0, width, height);

        OrthographicCamera& camera = OrthographicCamera::getInstance();
        camera.UpdateProjectionMatrix(width, height);
    }

    Window* Application::GetWindow()
    {
        return m_Window;
    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        Application& app = Application::GetInstance();
        app.OnResize(width, height);
    }
}