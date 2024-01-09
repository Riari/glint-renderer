#version 330 core

layout (location = 0) in vec3 position;

out vec4 vertexColour;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(0.4 * position.x, 0.4 * position.y, position.z, 1.0);
    vertexColour = vec4(clamp(position, 0.0f, 1.0f), 1.0f);
}
