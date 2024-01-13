#include "TextRenderer.h"

#include <iostream>

#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <OrthographicCamera.h>
#include <App/Application.h>

namespace Echo
{
    OrthographicCamera& camera = OrthographicCamera::getInstance();
    Application& app = Application::GetInstance();

    TextRenderer::TextRenderer()
    {

    }

    TextRenderer::~TextRenderer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }

    void TextRenderer::Init()
    {
        glm::vec2 defaultWindowSize = app.GetSettings().GetDefaultWindowSize();

        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        shader = Shader("text.vs", "text.fs");

        camera.setProjection(0.0f, static_cast<float>(defaultWindowSize.x), 0.0f, static_cast<float>(defaultWindowSize.y));

        shader.Activate();

        shader.SetShaderUniform4V("projection", camera.getProjectionMatrix());

        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return;
        }

        // load font as face
        FT_Face face;
        if (FT_New_Face(ft, app.GetSettings().GetDefaultFont().c_str(), 0, &face))
        {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            return;
        }
        else
        {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, 48);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            FT_GlyphSlot slot = face->glyph;

            float maxCharacterHeight = 0.0f;

            // load first 128 characters of ASCII set
            for (unsigned char c = 0; c < 128; c++)
            {
                // Load character glyph 
                maxCharacterHeight = std::max(maxCharacterHeight, (float)face->glyph->bitmap.rows);

                lineHeight = 1.2f * maxCharacterHeight;

                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                {
                    std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                    continue;
                }

                FT_Render_Glyph(slot, FT_RENDER_MODE_SDF);

                // generate texture
                unsigned int texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                Character character =
                {
                    texture,
                    glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                    glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                    static_cast<unsigned int>(face->glyph->advance.x)
                };

                Characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void TextRenderer::LoadFont(const std::string& fontPath) 
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return;
        }

        FT_Face face;
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) 
        {
            std::cout << "ERROR::FREETYPE: Failed to load font at " << fontPath << std::endl;
            return;
        }

        // set size to load glyphs as
        FT_Set_Pixel_Sizes(face, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        FT_GlyphSlot slot = face->glyph;

        float maxCharacterHeight = 0.0f;

        FontData fontData;

        for (unsigned char c = 0; c < 128; c++) 
        { 
            // Load first 128 characters of ASCII set
            if (FT_Load_Char(face, c, FT_LOAD_RENDER)) 
            {
                std::cout << "ERROR::FREETYPE: Failed to load Glyph for character " << c << std::endl;
                continue;
            }

            FT_Render_Glyph(slot, FT_RENDER_MODE_SDF);

            // Generate texture for the glyph
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Store character data
            Character character = 
            {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };

            fontData.Characters.insert(std::pair<char, Character>(c, character));
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);

        fonts[fontPath] = fontData;

        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    float TextRenderer::CalculateTextWidth(const std::string& text, float scale)
    {
        float width = 0.0f;

        for (const char& c : text)
        {
            Character ch = Characters[c]; // Assuming Characters is your glyph map
            float charWidth = (ch.Advance >> 6) * scale; // Current method
            // Possible adjustment
            // charWidth = someAdjustmentFunction(charWidth);
            width += charWidth;
        }

        return width;
    }

    float TextRenderer::CalculateTextHeight(const std::string& text, float scale, float containerWidth)
    {
        std::vector<std::string> lines = WrapText(text, scale, containerWidth);

        // Calculate the total height
        float totalHeight = 0.0f;

        // Add the height of each line
        for (const std::string& line : lines)
        {
            if (!line.empty())
            {
                // Assuming all characters are roughly the same height, 
                // use the height of the first character in the line.
                Character ch = Characters[line[0]];
                totalHeight += (ch.Size.y * scale);
            }
        }

        // Add additional spacing based on line height and number of lines
        // If you have different line heights for different lines, 
        // you'll need to adjust this calculation accordingly.
        totalHeight += (lineHeight * (lines.size() - 1));

        return totalHeight;
    }

    std::vector<std::string> TextRenderer::WrapText(std::string text, float scale, float containerWidth)
    {
        std::vector<std::string> lines;
        std::string currentLine;
        float currentWidth = 0.0f;

        std::istringstream wordsStream(text);
        std::string word;
        while (wordsStream >> word)
        {
            float spaceWidth = CalculateTextWidth(" ", scale);
            float wordWidth = CalculateTextWidth(word, scale);
            if (currentWidth + wordWidth > containerWidth)
            {
                if (!currentLine.empty())
                {
                    lines.push_back(currentLine);
                    currentLine = "";
                    currentWidth = 0.0f;
                }
            }
            if (!currentLine.empty())
            {
                currentLine += " ";
                currentWidth += spaceWidth;
            }
            currentLine += word;
            currentWidth += wordWidth;
        }

        if (!currentLine.empty())
        {
            lines.push_back(currentLine);
        }

        return lines;
    }

    void TextRenderer::SetLineHeight(float height)
    {
        lineHeight = height;
    }
 
    void TextRenderer::RenderText(const std::string& fontPath, const std::string& text, float x, float y, float scale, const glm::vec3& color, float minScale, float maxScale, TextAlignment alignment, float containerWidth, float dynamicScalingAmount)
    {
        // Load the font if not already loaded
        if (fonts.find(fontPath) == fonts.end()) 
        {
            LoadFont(fontPath);
        }

        // Select the font for rendering
        const FontData& font = fonts[fontPath];

        Application& app = Application::GetInstance();
        int windowWidth = app.GetWindow()->GetWindowSize().x, windowHeight = app.GetWindow()->GetWindowSize().y;

        std::vector<std::string> lines = WrapText(text, scale, containerWidth);
        y = std::min(y, static_cast<float>(windowHeight - CalculateTextHeight(text, scale, containerWidth)));

        if (y < 0)
        {
            y = 0;
        }      
        
        for (const std::string& line : lines)
        {
            float lineWidth = CalculateTextWidth(line, scale);
            float lineX = 0; // Initialize to 0 for left alignment

            // Boundary checks for left, center, and right alignments
            if (alignment == TextAlignment::CENTER)
            {
                lineX = std::max(0.0f, (windowWidth - lineWidth) / 2);
            }
            else if (alignment == TextAlignment::RIGHT)
            {
                lineX = std::max(0.0f, windowWidth - lineWidth);
            }
            else
            {
                lineX = std::min(std::max(x, 0.0f), windowWidth - lineWidth);
            }

            if (lineX < 0)
            {
                lineX = 0;
            }

            // Rendering the line
            RenderLine(font, line, lineX, y, scale, color, minScale, maxScale, dynamicScalingAmount);

            y -= lineHeight;
        }
    }

    void TextRenderer::RenderText(const std::string& text, float x, float y)
    {
        RenderText(app.GetSettings().GetDefaultFont(), text, x, y, 1.0f, glm::vec3(1.0f), 0.25f, 1.0f, TextAlignment::LEFT, 1000, 0.7f);
    }

    void TextRenderer::RenderText(const std::string& text, float x, float y, float scale)
    {
        RenderText(app.GetSettings().GetDefaultFont(), text, x, y, scale, glm::vec3(1.0f), 0.25f, 1.0f, TextAlignment::LEFT, 1000, 0.7f);
    }

    void TextRenderer::RenderText(const std::string& fontPath, const std::string& text, float x, float y, float scale)
    {
        RenderText(fontPath, text, x, y, scale, glm::vec3(1.0f), 0.25f, 1.0f, TextAlignment::LEFT, 1000, 0.7f);
    }

    void TextRenderer::RenderText(const std::string& fontPath, const std::string& text, float x, float y, float scale, const glm::vec3& color)
    {
        RenderText(fontPath, text, x, y, scale, color, 0.25f, 1.0f, TextAlignment::LEFT, 1000, 0.7f);
    }

    void TextRenderer::RenderText(const std::string& text, float x, float y, float scale, const glm::vec3& color)
    {
        RenderText(app.GetSettings().GetDefaultFont(), text, x, y, scale, color, 0.25f, 1.0f, TextAlignment::LEFT, 1000, 0.7f);
    }

    void TextRenderer::RenderLine(const FontData& font, std::string line, float x, float y, float scale, glm::vec3 color, float minScale, float maxScale, float dynamicScalingAmount)
    {
        Application& app = Application::GetInstance();
        int windowWidth = app.GetWindow()->GetWindowSize().x;
        int windowHeight = app.GetWindow()->GetWindowSize().y;

        const float referenceWidth = app.GetSettings().GetDefaultWindowSize().x; // inital width of the window
        const float referenceHeight = app.GetSettings().GetDefaultWindowSize().y; // inital height of the window

        // Calculate scale factors for width and height
        float scaleW = static_cast<float>(windowWidth) / referenceWidth;
        float scaleH = static_cast<float>(windowHeight) / referenceHeight;
        float windowScale = std::min(scaleW, scaleH);

        // Blend the user scale and window scale
        float finalScale = (1.0f - dynamicScalingAmount) * scale + dynamicScalingAmount * windowScale * scale;

        // Clamp the scale between minScale and maxScale
        finalScale = std::max(finalScale, minScale);
        finalScale = std::min(finalScale, maxScale);

        // Activate shader and set text color
        shader.Activate();

        shader.SetShaderUniform4V("projection", camera.getProjectionMatrix());

        glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);

        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // Iterate through all characters in the line
        for (const char& c : line)
        {
            if (font.Characters.find(c) == font.Characters.end()) {
                std::cerr << "Character '" << c << "' not found in font data." << std::endl;
                continue; // Skip this character
            }

            Character ch = font.Characters.at(c);

            float xpos = x + ch.Bearing.x * finalScale;
            float ypos = y - (ch.Size.y - ch.Bearing.y) * finalScale;

            float w = ch.Size.x * finalScale;
            float h = ch.Size.y * finalScale;

            // Update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };

            // Render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // Update content of VBO memory
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            // Render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
       
            // Now advance cursors for next glyph
            x += (ch.Advance >> 6) * finalScale; // Bitshift by 6 to get value in pixels
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}