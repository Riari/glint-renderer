#version 450 core

in vec4 vertexColour;
in vec2 vertexUVs;

out vec4 colour;

struct DirectionalLight
{
    vec3 colour;
    float ambientIntensity;
};

uniform sampler2D textureSampler;
uniform DirectionalLight directionalLight;

void main()
{
    vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;
    colour = texture(textureSampler, vertexUVs) * ambientColour;
}
