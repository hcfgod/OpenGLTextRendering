#pragma once

#include <string>
#include <map>
#include <vector>

#include <GLM/ext/matrix_clip_space.hpp>
#include <GLM/vec2.hpp>
#include <GLM/vec3.hpp>

#include <Shader.h>

namespace Echo
{
    struct Character
    {
        unsigned int TextureID; // ID handle of the glyph texture
        glm::ivec2   Size;      // Size of glyph
        glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
        unsigned int Advance;   // Horizontal offset to advance to next glyph
    };

    struct FontData
    {
        std::map<GLchar, Character> Characters;
        // Other font-related data (like texture ID if needed)
    };

    enum class TextAlignment
    {
        LEFT,
        CENTER,
        RIGHT
    };

    class TextRenderer
    {
    public:
        TextRenderer();
        ~TextRenderer();

        void Init();

        void LoadFont(const std::string& fontPath);

        float CalculateTextWidth(const std::string& text, float scale);
        float CalculateTextHeight(const std::string& text, float scale, float containerWidth);

        std::vector<std::string> WrapText(std::string text, float scale, float containerWidth);
        void SetLineHeight(float height);

        void RenderText(std::string text, const std::string& fontPath, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), float minScale = 0.25f, float maxScale = 1.0f, TextAlignment alignment = TextAlignment::LEFT, float containerWidth = 1000, float dynamicScalingAmount = 0.7f);
    private:
        std::vector<float> vertexData;
        std::map<GLchar, Character> Characters{};
        std::map<std::string, FontData> fonts;
        std::vector<std::string> lines;
        float lineHeight = 0;
        unsigned int VAO = 0, VBO = 0;
        Shader shader;

        void RenderLine(const FontData& font, std::string line, float x, float y, float scale, glm::vec3 color, float minScale, float maxScale, float dynamicScalingAmount);
    };
}