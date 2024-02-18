#include "GL.h"

#include <string>
#include <unordered_map>

#include <spdlog/spdlog.h>

namespace Util::GL
{
    void APIENTRY HandleDebugMessage(GLenum source,
                                GLenum type,
                                unsigned int id,
                                GLenum severity,
                                GLsizei length,
                                const char *message,
                                const void *userParam)
    {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
            return;

        static const std::unordered_map<GLenum, std::string> sourceStrings = {
            {GL_DEBUG_SOURCE_API, "API"},
            {GL_DEBUG_SOURCE_WINDOW_SYSTEM, "Window System"},
            {GL_DEBUG_SOURCE_SHADER_COMPILER, "Shader Compiler"},
            {GL_DEBUG_SOURCE_THIRD_PARTY, "Third Party"},
            {GL_DEBUG_SOURCE_APPLICATION, "Application"},
            {GL_DEBUG_SOURCE_OTHER, "Other"},
        };

        static const std::unordered_map<GLenum, std::string> typeStrings = {
            {GL_DEBUG_TYPE_ERROR, "Error"},
            {GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR, "Deprecated Behaviour"},
            {GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR, "Undefined Behaviour"},
            {GL_DEBUG_TYPE_PORTABILITY, "Portability"},
            {GL_DEBUG_TYPE_PERFORMANCE, "Performance"},
            {GL_DEBUG_TYPE_MARKER, "Marker"},
            {GL_DEBUG_TYPE_PUSH_GROUP, "Push Group"},
            {GL_DEBUG_TYPE_POP_GROUP, "Pop Group"},
            {GL_DEBUG_TYPE_OTHER, "Other"},
        };

        static const std::unordered_map<GLenum, std::string> severityStrings = {
            {GL_DEBUG_SEVERITY_HIGH, "High"},
            {GL_DEBUG_SEVERITY_MEDIUM, "Medium"},
            {GL_DEBUG_SEVERITY_LOW, "Low"},
            {GL_DEBUG_SEVERITY_NOTIFICATION, "Info"},
        };

        spdlog::error(
            "[GL] [{}] [{}] [{}] {} (ID: {})",
            sourceStrings.at(source),
            typeStrings.at(type),
            severityStrings.at(severity),
            message,
            id);
    }
}

