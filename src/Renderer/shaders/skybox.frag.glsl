#version 330

in vec3 vertexUV;

out vec4 fragColor;

uniform samplerCube skybox;

void main()
{
	fragColor = texture(skybox, vertexUV);
}
