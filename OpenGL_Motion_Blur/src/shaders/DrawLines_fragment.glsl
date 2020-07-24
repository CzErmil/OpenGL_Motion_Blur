#version 330 core

in vec3 gs_color;

out vec4 color;

void main(void)
{
	color = vec4(gs_color,1);
}