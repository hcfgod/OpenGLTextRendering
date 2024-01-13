#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class OrthographicCamera 
{
public:
    static OrthographicCamera& getInstance()
    {
        static OrthographicCamera instance;
        return instance;
    }

    OrthographicCamera(OrthographicCamera const&) = delete;
    void operator=(OrthographicCamera const&) = delete;

    void UpdateProjectionMatrix(int width, int height)
    {
        setProjection(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
    }

    void setProjection(float left, float right, float bottom, float top)
    {
        projectionMatrix = glm::ortho(left, right, bottom, top);
    }

    const glm::mat4& getProjectionMatrix() const 
    {
        return projectionMatrix;
    }

private:
    glm::mat4 projectionMatrix;

    OrthographicCamera() 
    {
        // Initialize with default values
        setProjection(-1.0f, 1.0f, -1.0f, 1.0f);
    }
};