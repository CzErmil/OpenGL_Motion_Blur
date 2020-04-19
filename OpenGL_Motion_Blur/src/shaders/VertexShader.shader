#version 330 core

layout (location = 0) in vec4 position;

uniform mat4 u_MVP;
uniform vec4 u_color;

out vec4 vs_color;

void main(void)
{
	gl_Position = u_MVP * position;

	vs_color = u_color;
}