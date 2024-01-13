#pragma once
#include <GL/glew.h>

#include <App/Window/Window.h>
#include <TextRenderer.h>
#include "ApplicationSettings.h"

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

		ApplicationSettings& GetSettings();

		void Initialize(const char* title, int initalWindowWidth, int initalWindowHeight);
		void Run();
		void Awake();
		void Render();
		void OnResize(int width, int height);

		Window* GetWindow();
	private:
		Application();
		~Application();

		ApplicationSettings settings;

		Window* m_Window = nullptr;
		TextRenderer textRenderer;
	};
}