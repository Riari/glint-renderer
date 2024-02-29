#version 330 core

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D textureSampler;

void main()
{
    // Sample the depth map
    float depthValue = texture(textureSampler, fragTexCoord).r;

    // Map depth to color gradient (adjust the near and far values accordingly)
    float near = 0.0; // Near plane distance
    float far = 1.0;  // Far plane distance

    float normalizedDepth = (depthValue - near) / (far - near);

    // Visualize depth using a gradient from blue (near) to red (far)
    vec3 color = vec3(1.0 - normalizedDepth, 1.0 - normalizedDepth, 1.0 - normalizedDepth);

    fragColor = vec4(color, 1.0);
}
