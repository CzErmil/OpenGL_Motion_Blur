#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;

uniform mat4 u_MVP;
uniform mat4 u_Model;

out vec3 vs_normal;
out vec2 vs_textureCoord;
out vec3 vs_fragCoord;

void main(void)
{
	gl_Position = u_MVP * position;

	vs_normal = vec3(mat3(u_Model) * normal);
	vs_textureCoord = textureCoord;
	vs_fragCoord = vec3(u_Model * position);
}