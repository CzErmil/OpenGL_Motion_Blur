#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

in vec3 vs_color[];
out vec3 gs_color;

void main(void)
{

	for(int i = 0; i < gl_in.length(); i++)
	{
		gl_Position = gl_in[i].gl_Position;
		EmitVertex();
		gs_color = vs_color[0];
	}
}