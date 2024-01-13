#include "ApplicationSettings.h"

namespace Echo
{
    ApplicationSettings::ApplicationSettings() : defaultWindowSize(1280, 720), defaultFont("amiri-regular.ttf"), vsyncEnabled(true)
    {
    }

    void ApplicationSettings::SetDefaultWindowSize(const glm::vec2& size)
    {
        defaultWindowSize = size;
    }

    glm::vec2 ApplicationSettings::GetDefaultWindowSize() const
    {
        return defaultWindowSize;
    }

    void ApplicationSettings::SetDefaultFont(const std::string& font)
    {
        defaultFont = font;
    }

    std::string ApplicationSettings::GetDefaultFont() const
    {
        return defaultFont;
    }

    void ApplicationSettings::SetVSyncEnabled(bool enabled)
    {
        vsyncEnabled = enabled;
    }

    bool ApplicationSettings::IsVSyncEnabled() const
    {
        return vsyncEnabled;
    }
}