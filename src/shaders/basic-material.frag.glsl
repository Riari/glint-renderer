#version 330 core

in vec4 vertexColour;
in vec2 vertexUV;
in vec3 vertexNormal;
in vec3 vertexFragPosition;

out vec4 colour;

const int MAX_POINT_LIGHTS = 32;

struct Light
{
    vec3 colour;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};

struct PointLight
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;

uniform sampler2D textureSampler;
uniform Material material;

uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 direction)
{
    vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

    float diffuseFactor = max(dot(normalize(vertexNormal), normalize(direction)), 0.0f);
    vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);

    vec4 specularColour = vec4(0, 0, 0, 0);

    if (diffuseFactor > 0.0f)
    {
        vec3 fragToEye = normalize(eyePosition - vertexFragPosition);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(vertexNormal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if (specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4(light.colour * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    return (ambientColour + diffuseColour + specularColour);
}

vec4 CalcDirectionalLight()
{
    return CalcLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalcPointLights()
{
    vec4 totalColour = vec4(0, 0, 0, 0);
    for (int i = 0; i < pointLightCount; ++i)
    {
        vec3 direction = vertexFragPosition - pointLights[i].position;
        float distance = length(direction);
        direction = normalize(direction);

        vec4 colour = CalcLightByDirection(pointLights[i].base, direction);
        float attenuation = pointLights[i].exponent * distance * distance +
                            pointLights[i].linear * distance +
                            pointLights[i].constant;

        totalColour += (colour / attenuation);
    }

    return totalColour;
}

void main()
{
    vec4 finalColour = CalcDirectionalLight();
    finalColour += CalcPointLights();

    colour = texture(textureSampler, vertexUV) * finalColour;
}
