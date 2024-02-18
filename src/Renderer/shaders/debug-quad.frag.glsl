#version 330 core

in vec2 vertexPosition;

out vec4 fragColour;

uniform sampler2D textureSampler;

float LinearizeDepth(vec2 uv)
{
    float zNear = 0.1;
    float zFar  = 100.0;
    float depth = texture2D(textureSampler, uv).x;
    return (2.0 * zNear) / (zFar + zNear - depth * (zFar - zNear));
}

void main()
{
    float colour = LinearizeDepth(vertexPosition);
    fragColour = vec4(colour, colour, colour, 1.0);
}
