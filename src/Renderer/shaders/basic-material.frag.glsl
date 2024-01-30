#version 330

in vec2 vertexUV;
in vec3 vertexNormal;
in vec3 vertexFragPosition;

out vec4 colour;

const int MAX_POINT_LIGHTS = 32;
const int MAX_SPOT_LIGHTS = 32;

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

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float edgeCosine;
};

struct Material
{
    bool useTexture;
    vec3 baseColour;
    float specularIntensity;
    float shininess;
};

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int spotLightCount;

uniform sampler2D textureSampler;
uniform Material material;

uniform vec3 eyePosition;

vec4 CalcLightByDirection(Light light, vec3 direction)
{
    vec4 ambientColour = vec4(light.colour, 1.0f) * light.ambientIntensity;

    float diffuseFactor = max(dot(normalize(vertexNormal), normalize(direction)), 0.0f);
    vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0f);

    vec4 specularColour = vec4(0);

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

vec4 CalcPointLight(PointLight pointLight)
{
    vec3 direction = vertexFragPosition - pointLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 colour = CalcLightByDirection(pointLight.base, direction);
    float attenuation = pointLight.exponent * distance * distance +
                        pointLight.linear * distance +
                        pointLight.constant;

    return colour / attenuation;
}

vec4 CalcPointLights()
{
    vec4 totalColour = vec4(0);
    for (int i = 0; i < pointLightCount; ++i)
    {
        totalColour += CalcPointLight(pointLights[i]);
    }

    return totalColour;
}

vec4 CalcSpotLight(SpotLight spotLight)
{
    vec3 rayDirection = normalize(vertexFragPosition - spotLight.base.position);
    float factor = dot(rayDirection, spotLight.direction);

    if (factor > spotLight.edgeCosine)
    {
        vec4 colour = CalcPointLight(spotLight.base);

        return colour * (1.0f - (1.0f - factor) * (1.0f / (1.0f - spotLight.edgeCosine)));
    }

    return vec4(0);
}

vec4 CalcSpotLights()
{
    vec4 totalColour = vec4(0);
    for (int i = 0; i < spotLightCount; ++i)
    {
        totalColour += CalcSpotLight(spotLights[i]);
    }

    return totalColour;
}

void main()
{
    vec4 finalColour = CalcLightByDirection(directionalLight.base, directionalLight.direction);
    finalColour += CalcPointLights();
    finalColour += CalcSpotLights();

    if (material.useTexture)
    {
        colour = texture(textureSampler, vertexUV) * finalColour;
    }
    else
    {
        colour = vec4(material.baseColour, 1.0f) * finalColour;
    }
}
