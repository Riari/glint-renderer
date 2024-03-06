#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

uniform mat4 lightMatrices[6];

out vec4 vertexFragPosition;

void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face;

        for (int i = 0; i < 3; ++i)
        {
            vertexFragPosition = gl_in[i].gl_Position;
            gl_Position = lightMatrices[face] * vertexFragPosition;
            EmitVertex();
        }

        EndPrimitive();
    }
}
