#version 450 core

in vec4 vertexColour;
in vec2 vertexUVs;

out vec4 colour;

uniform sampler2D textureSampler;

void main()
{
    colour = texture(textureSampler, vertexUVs) * vertexColour;
}
