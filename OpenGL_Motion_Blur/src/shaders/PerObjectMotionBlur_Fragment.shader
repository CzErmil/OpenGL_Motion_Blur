#version 330 core

in vec2 vs_textureCoord;

uniform sampler2D u_texture;
uniform sampler2D u_velocityBuffer;

uniform int u_blurLevel;

out vec4 color;

void main(void)
{
	vec2 blurVec = texture(u_velocityBuffer, vs_textureCoord).xy;

	int n = u_blurLevel;

	vec4 result = vec4(0, 0, 0, 0);

	for (int i = 0; i < n; i++)
	{
		vec2 offset = blurVec * (float(i) / float(n - 1) - 0.5);

		result += texture(u_texture, vs_textureCoord + offset);
	}

	result /= float(n);

	color = result;
}