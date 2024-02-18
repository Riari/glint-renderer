#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

out vec2 vertexUV;
out vec3 vertexNormal;
out vec3 vertexFragPosition;
out vec4 vertexDirectionalLightSpacePosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 directionalLightTransform;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0);
    vertexDirectionalLightSpacePosition = directionalLightTransform * model * vec4(position, 1.0);

    vertexUV = uv;
    vertexNormal = mat3(transpose(inverse(model))) * normal;
    vertexFragPosition = (model * vec4(position, 1.0)).xyz;
}
