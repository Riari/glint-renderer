#version 330 core

in vec4 vertexColour;
in vec2 vertexUV;
in vec3 vertexNormal;
in vec3 vertexFragPosition;

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

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform sampler2D textureSampler;

uniform Light ambientLight;
uniform DirectionalLight directionalLight;

uniform Material material;

uniform vec3 eyePosition;

void main()
{
    vec4 ambientColour = vec4(ambientLight.colour, 1.0f) * ambientLight.intensity;

    float diffuseFactor = max(dot(normalize(vertexNormal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.intensity * diffuseFactor;

    vec4 specularColour = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0.0f)
    {
        vec3 fragToEye = normalize(eyePosition - vertexFragPosition);
        vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(vertexNormal)));

        float specularFactor = dot(fragToEye, reflectedVertex);

        if (specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    colour = texture(textureSampler, vertexUV) * (ambientColour + diffuseColour + specularColour);
}
