#pragma once

#include <GLFW/glfw3.h>
#include "glm.hpp"

namespace glimac
{

    struct ShapeVertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoords;
        glm::vec3 color;

        ShapeVertex() {}
        ShapeVertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texCoords)
            : position(position), normal(normal), texCoords(texCoords), color(glm::vec3(1, 1, 1)) {}
        ShapeVertex(const glm::vec3 &position, const glm::vec3 &normal, const glm::vec2 &texCoords, const glm::vec3 &color)
            : position(position), normal(normal), texCoords(texCoords), color(color) {}
    };

}
