#include <App/Application.h>

int main()
{
    Echo::Application& app = Echo::Application::GetInstance();
    app.Run();
}