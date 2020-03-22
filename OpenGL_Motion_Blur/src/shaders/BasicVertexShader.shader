#version 330 core

layout (location = 0) in vec4 position;

out vec4 vs_color;

void main(void)
{
	gl_Position = position;

	vs_color = vec4(0.6, 0.5, 0.2, 1.0);
}