#pragma once

#include <GLFW/glfw3.h>
#include <GLM/ext/vector_float2.hpp>

namespace Echo
{
	class Window
	{
	public:
		Window(const char* title, glm::vec2 initalWindowSize);
		~Window();

		bool ShouldWindowClose();
		void SetShouldWindowClose(bool value);

		glm::vec2 GetWindowSize();

		GLFWwindow* GetGlfwWindow();
	private:
		GLFWwindow* m_Window = nullptr;
	};
}