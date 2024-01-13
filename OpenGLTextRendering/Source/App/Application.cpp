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

        settings.SetDefaultWindowSize(m_Window->GetWindowSize());
	}

	Application::~Application()
	{
		delete m_Window;

        glfwTerminate();
	}

    Window* Application::GetWindow()
    {
        return m_Window;
    }

    ApplicationSettings& Application::GetSettings()
    {
        return settings;
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

        textRenderer.RenderText("This is a sample text", 5, 55, 0.5);
        textRenderer.RenderText("Raleway-Italic.ttf", "This is a sample text", 5, 5, 0.5f, glm::vec3(1), 0.25f, 1.0f, TextAlignment::LEFT, 1000, 0.5f);

        glfwSwapBuffers(m_Window->GetGlfwWindow());
        glfwPollEvents();
	}

    void Application::OnResize(int width, int height)
    {
        glViewport(0, 0, width, height);

        OrthographicCamera& camera = OrthographicCamera::getInstance();
        camera.UpdateProjectionMatrix(width, height);
    }

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        Application& app = Application::GetInstance();
        app.OnResize(width, height);
    }
}