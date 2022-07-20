#version 330 core

#define MAX_FRAME_LAYERS 100

layout (triangles) in;
layout (line_strip, max_vertices = 3) out;

uniform mat4 u_MVP[MAX_FRAME_LAYERS];

in vec3 vs_color[];
in int vs_instanceID[];
out vec3 gs_color;

void main(void)
{
	for(int i = 0; i < gl_in.length(); i++)
	{
		gl_Layer = vs_instanceID[0];
		gl_Position = u_MVP[vs_instanceID[0]] * gl_in[i].gl_Position;

		gs_color = vs_color[0];

		EmitVertex();
	}
	EndPrimitive();

}