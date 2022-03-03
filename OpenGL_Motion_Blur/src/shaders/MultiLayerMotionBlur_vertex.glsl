#version 410 core

#define FRAME_LAYERS 16

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;

uniform mat4 u_MVP[FRAME_LAYERS];
uniform mat4 u_Model[FRAME_LAYERS];

out vec3 vs_normal;
out vec2 vs_textureCoord;

void main(void)
{
	gl_Position = position;

	vs_normal = normal;
	vs_textureCoord = textureCoord;
}