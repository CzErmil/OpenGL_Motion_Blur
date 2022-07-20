#version 430 core

#define MAX_FRAME_LAYERS 100

layout( triangles ) in;
layout( triangle_strip, max_vertices = 3 ) out;

uniform mat4 modelViewProjectionMatrix[MAX_FRAME_LAYERS];

uniform mat4 u_MVP[MAX_FRAME_LAYERS];
uniform mat4 u_Model[MAX_FRAME_LAYERS];

in vec3 vs_normal[];
in vec2 vs_textureCoord[];
in int vs_instanceID[];

out vec3 gs_normal;
out vec2 gs_textureCoord;
out vec3 gs_fragCoord;

void main(void)
{
	for(int i = 0; i < gl_in.length(); i++)
	{
		gl_Layer = vs_instanceID[0];

		gl_Position = u_MVP[vs_instanceID[0]] * gl_in[i].gl_Position;

		
		gs_normal = vec3(mat3(u_Model[vs_instanceID[0]]) * vs_normal[i]);
		gs_textureCoord = vs_textureCoord[i];
		gs_fragCoord = vec3(u_Model[vs_instanceID[0]] * gl_in[i].gl_Position);

		EmitVertex();
	}
	EndPrimitive();
}