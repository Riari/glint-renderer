#pragma once

#include <vector>

#include <GL/glew.h>

namespace Renderer
{
    const int VIEWPORT_WIDTH{1920};
    const int VIEWPORT_HEIGHT{1080};

    const float NEAR_PLANE = 0.1f;
    const float FAR_PLANE = 1000.0f;

    const int SHADOW_MAP_RESOLUTION = 4096;

    // Primitives
    const std::vector<GLfloat> PRIMITIVE_PLANE_VERTICES = {
        -10.0f, 0.0f, -10.0f,   0.0f, 0.0f,     0.0f, 1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,    10.0f, 0.0f,    0.0f, 1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f,    0.0f, 10.0f,    0.0f, 1.0f, 0.0f,
        10.0f, 0.0f, 10.0f,     10.0f, 10.0f,   0.0f, 1.0f, 0.0f,
    };

    const std::vector<GLuint> PRIMITIVE_PLANE_INDICES = {
        0, 2, 1,
        1, 2, 3,
    };

    const std::vector<GLfloat> PRIMITIVE_SKYBOX_VERTICES = {
        -1.0f, 1.0f, -1.0f,     0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f,    0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f,      0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -1.0f,     0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f,      0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,       0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f,     0.0f, 0.0f,     0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f,      0.0f, 0.0f,     0.0f, 0.0f, 0.0f
    };

    const std::vector<GLuint> PRIMITIVE_SKYBOX_INDICES = {
        // front
        0, 1, 2,
        2, 1, 3,
        // right
        2, 3, 5,
        5, 3, 7,
        // back
        5, 7, 4,
        4, 7, 6,
        // left
        4, 6, 0,
        0, 6, 1,
        // top
        4, 0, 5,
        5, 0, 2,
        // bottom
        1, 6, 3,
        3, 6, 7
    };

    const std::vector<GLfloat> PRIMITIVE_QUAD_VERTICES = {
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f, // Top-left
        1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // Top-right
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // Bottom-right
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f  // Bottom-left
    };

    const std::vector<GLuint> PRIMITIVE_QUAD_INDICES = {
        0, 1, 2,
        2, 3, 0,
    };
}
