#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoord;

uniform mat4 u_MVP;

out vec4 vs_color;
out vec2 vs_textureCoord;

void main(void)
{
	gl_Position = u_MVP * position;

	vs_textureCoord = textureCoord;
}