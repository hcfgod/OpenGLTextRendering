#pragma once
#include <GL/glew.h>

#include <App/Window/Window.h>
#include <TextRenderer.h>

namespace Echo
{
	class Application
	{
	public:
		static Application& GetInstance()
		{
			static Application instance;
			return instance;
		}

		void Run();
		void Awake();
		void Render();
		void OnResize(int width, int height);

		Window* GetWindow();
	private:
		Application();
		~Application();

		Window* m_Window;
		TextRenderer textRenderer;
	};
}