#version 330 core

in vec4 vs_color;
in vec2 vs_textureCoord;

uniform float u_alpha;
uniform sampler2D u_texture;

out vec4 color;

void main(void)
{
	color = vec4(texture(u_texture, vs_textureCoord).xyz, u_alpha);
}