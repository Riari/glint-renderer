#version 450 core

in vec4 vertexColour;
in vec2 vertexUV;
in vec3 vertexNormal;

out vec4 colour;

struct Light
{
    vec3 colour;
    float intensity;
};

struct DirectionalLight
{
    vec3 colour;
    float intensity;
    vec3 direction;
};

uniform sampler2D textureSampler;
uniform Light ambientLight;
uniform DirectionalLight directionalLight;

void main()
{
    vec4 ambientColour = vec4(ambientLight.colour, 1.0f) * ambientLight.intensity;

    float diffuseFactor = max(dot(normalize(vertexNormal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.intensity * diffuseFactor;

    colour = texture(textureSampler, vertexUV) * (ambientColour + diffuseColour);
}
