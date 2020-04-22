#version 330 core

in vec2 vs_textureCoord;

uniform sampler2D u_texture;

out vec4 color;

void main(void)
{
	float depthValue = texture(u_texture, vs_textureCoord).r;
	color = vec4(vec3(depthValue),1.0);
}