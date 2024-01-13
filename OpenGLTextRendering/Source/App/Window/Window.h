#pragma once

#include <GLFW/glfw3.h>
#include <GLM/ext/vector_float2.hpp>

namespace Echo
{
	class Window
	{
	public:
		Window();
		~Window();

		bool ShouldWindowClose();
		void SetShouldWindowClose(bool value);

		glm::vec2 GetWindowSize();

		GLFWwindow* GetGlfwWindow();
	private:
		GLFWwindow* m_Window;
	};
}