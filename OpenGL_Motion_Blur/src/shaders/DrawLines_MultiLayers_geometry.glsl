#version 330 core

#define FRAME_LAYERS 16
#define MAX_VERTICES 48 // 3 * FRAME_LAYERS

layout (triangles) in;
layout (line_strip, max_vertices = MAX_VERTICES) out;

uniform mat4 u_MVP[FRAME_LAYERS];

in vec3 vs_color[];
out vec3 gs_color;

void main(void)
{
	for(int layer = 0; layer<FRAME_LAYERS; layer++)
	{
		for(int i = 0; i < gl_in.length(); i++)
		{
			gl_Layer = layer;
			gl_Position = u_MVP[layer] * gl_in[i].gl_Position;

			gs_color = vs_color[0];

			EmitVertex();
		}
		EndPrimitive();
	}
}