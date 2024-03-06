#version 330

in vec4 vertexFragPosition;

uniform vec3 lightPosition;
uniform float farPlane;

void main()
{
    float distance = length(vertexFragPosition.xyz - lightPosition);
    distance = distance / farPlane;
    gl_FragDepth = distance;
}
