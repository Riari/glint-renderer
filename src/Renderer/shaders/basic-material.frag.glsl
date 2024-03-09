#version 330

in vec2 vertexUV;
in vec3 vertexNormal;
in vec3 vertexFragPosition;
in vec4 vertexDirectionalLightSpacePosition;

out vec4 outColour;

const int MAX_POINT_LIGHTS = 32;
const int MAX_SPOT_LIGHTS = 32;
const int GRID_SAMPLES = 20;

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
    bool useDiffuseTexture;
    sampler2D diffuseTexture;
    vec3 diffuseColour;

    bool useSpecularTexture;
    sampler2D specularTexture;
    vec3 specularColour;

    float specularIntensity;
    float shininess;
};

struct OmniShadowMap
{
    samplerCube map;
};

uniform DirectionalLight directionalLight;
uniform sampler2D directionalShadowMap;

uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int pointLightCount;
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
uniform int spotLightCount;
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform float farPlane;

uniform Material material;

uniform vec3 eyePosition;

vec3 gridSamplingDisk[GRID_SAMPLES] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1),
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

float CalcDirectionalShadowFactor(DirectionalLight light)
{
    vec3 projection = vertexDirectionalLightSpacePosition.xyz / vertexDirectionalLightSpacePosition.w;
    projection = (projection * 0.5) + 0.5;

    float current = projection.z;

    if (current > 1.0)
    {
        return 0.0;
    }

    vec3 normal = -normalize(vertexNormal);
    vec3 lightDirection = normalize(directionalLight.direction);

    float bias = max(0.010 * (1.0 - dot(normal, lightDirection)), 0.0003);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(directionalShadowMap, projection.xy + vec2(x, y) * texelSize).r;
            shadow += current - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    return shadow;
}

float CalcOmnidirectionalShadowFactor(PointLight light, int shadowMapIndex)
{
    vec3 fragToLight = vertexFragPosition - light.position;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.01;
    float viewDistance = length(eyePosition - vertexFragPosition);
    float diskRadius = (1.0 + (viewDistance / farPlane)) / 25.0;

    for (int i = 0; i < GRID_SAMPLES; ++i)
    {
        float closestDepth = texture(omniShadowMaps[shadowMapIndex].map, fragToLight + gridSamplingDisk[i] * diskRadius).r;
        closestDepth *= farPlane;
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }

    shadow /= float(GRID_SAMPLES);

    return shadow;
}

vec4 CalcLightByDirection(Light light, vec3 direction, float shadowFactor)
{
    vec4 ambientColour = vec4(light.colour, 1.0) * light.ambientIntensity;

    float diffuseFactor = max(dot(-normalize(vertexNormal), normalize(direction)), 0.0);
    vec4 diffuseColour = vec4(light.colour * light.diffuseIntensity * diffuseFactor, 1.0);

    vec4 specularColour = vec4(0);

    if (diffuseFactor > 0.0)
    {
        vec3 fragToEye = normalize(eyePosition - vertexFragPosition);
        vec3 reflectedVertex = normalize(reflect(direction, -normalize(vertexNormal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if (specularFactor > 0.0)
        {
            specularFactor = pow(specularFactor, material.shininess);

            vec3 specularBase = material.useSpecularTexture
                ? texture(material.specularTexture, vertexUV).rgb
                : material.specularColour;

            specularColour = vec4(light.colour * specularBase * material.specularIntensity * specularFactor, 1.0);
        }
    }

    return (ambientColour + (1.0 - shadowFactor) * (diffuseColour + specularColour));
}

vec4 CalcDirectionalLight()
{
    float shadowFactor = CalcDirectionalShadowFactor(directionalLight);
    return CalcLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 CalcPointLight(PointLight pointLight, int shadowMapIndex)
{
    vec3 direction = vertexFragPosition - pointLight.position;
    float lightToFragLength = length(direction);
    direction = normalize(direction);

    float shadowFactor = CalcOmnidirectionalShadowFactor(pointLight, shadowMapIndex);

    vec4 colour = CalcLightByDirection(pointLight.base, direction, shadowFactor);
    float attenuation = pointLight.exponent * lightToFragLength * lightToFragLength +
                        pointLight.linear * lightToFragLength +
                        pointLight.constant;

    return colour / attenuation;
}

vec4 CalcPointLights()
{
    vec4 totalColour = vec4(0);
    for (int i = 0; i < pointLightCount; ++i)
    {
        totalColour += CalcPointLight(pointLights[i], i);
    }

    return totalColour;
}

vec4 CalcSpotLight(SpotLight spotLight, int shadowMapIndex)
{
    vec3 rayDirection = normalize(vertexFragPosition - spotLight.base.position);
    float factor = dot(rayDirection, spotLight.direction);

    if (factor > spotLight.edgeCosine)
    {
        vec4 colour = CalcPointLight(spotLight.base, shadowMapIndex);
        return colour * (1.0 - (1.0 - factor) * (1.0 / (1.0 - spotLight.edgeCosine)));
    }

    return vec4(0);
}

vec4 CalcSpotLights()
{
    vec4 totalColour = vec4(0);
    for (int i = 0; i < spotLightCount; ++i)
    {
        totalColour += CalcSpotLight(spotLights[i], pointLightCount + i);
    }

    return totalColour;
}

void main()
{
    vec4 finalColour = CalcDirectionalLight();
    finalColour += CalcPointLights();
    finalColour += CalcSpotLights();

    outColour = material.useDiffuseTexture
        ? texture(material.diffuseTexture, vertexUV) * finalColour
        : vec4(material.diffuseColour, 1.0) * finalColour;
}
