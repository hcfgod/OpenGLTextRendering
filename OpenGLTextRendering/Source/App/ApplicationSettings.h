#pragma once

#include <string>
#include <GLM/vec2.hpp>

namespace Echo
{
    class ApplicationSettings
    {
    public:
        ApplicationSettings();

        void SetDefaultWindowSize(const glm::vec2& size);
        glm::vec2 GetDefaultWindowSize() const;

        void SetDefaultFont(const std::string& font);
        std::string GetDefaultFont() const;

        void SetVSyncEnabled(bool enabled);
        bool IsVSyncEnabled() const;

    private:
        glm::vec2 defaultWindowSize;
        std::string defaultFont;
        bool vsyncEnabled;
    };
}