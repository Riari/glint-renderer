#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec4 vertexColour;
out vec2 vertexUV;
out vec3 vertexNormal;
out vec3 vertexFragPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    vertexColour = vec4(clamp(position, 0.0f, 1.0f), 1.0f);

    vertexUV = uv;
    vertexNormal = mat3(transpose(inverse(model))) * normal;
    vertexFragPosition = (model * vec4(position, 1.0)).xyz;
}
