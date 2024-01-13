#include <App/Application.h>

int main()
{
    Echo::Application& app = Echo::Application::GetInstance();
    app.Initialize("OpenGL Text Rendering", 1280, 720);
    app.Run();
}