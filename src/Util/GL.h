#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Util::GL
{
    void APIENTRY HandleDebugMessage(GLenum source,
                                    GLenum type,
                                    unsigned int id,
                                    GLenum severity,
                                    GLsizei length,
                                    const char *message,
                                    const void *userParam);

    GLint CheckShaderBuildStatus(GLuint objectId, int type);
}
