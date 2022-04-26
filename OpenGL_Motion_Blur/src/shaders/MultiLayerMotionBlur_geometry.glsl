#version 410 core

#define FRAME_LAYERS 16
#define MAX_VERTICES 48 // 3 * FRAME_LAYERS

layout( triangles ) in;
layout( triangle_strip, max_vertices = MAX_VERTICES ) out;

uniform mat4 modelViewProjectionMatrix[16];

uniform mat4 u_MVP[FRAME_LAYERS];
uniform mat4 u_Model[FRAME_LAYERS];

in vec3 vs_normal[];
in vec2 vs_textureCoord[];

out vec3 gs_normal;
out vec2 gs_textureCoord;
out vec3 gs_fragCoord;

void main(void)
{
	for(int layer = 0; layer<FRAME_LAYERS; layer++)
	{
		for(int i = 0; i < gl_in.length(); i++)
		{
			gl_Layer = layer;

			gl_Position = u_MVP[layer] * gl_in[i].gl_Position;

		
			gs_normal = vec3(mat3(u_Model[layer]) * vs_normal[i]);
			gs_textureCoord = vs_textureCoord[i];
			gs_fragCoord = vec3(u_Model[layer] * gl_in[i].gl_Position);

			EmitVertex();
		}
		EndPrimitive();
	}

}