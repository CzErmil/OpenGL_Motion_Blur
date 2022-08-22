#version 400 core

in vec2 vs_textureCoord;

uniform sampler2D u_texture;
uniform sampler2D u_velocityBuffer;

uniform int u_blurLevel;
uniform float u_blurPower;

out vec4 color;

void main(void)
{
	vec2 texelSize = 1.0 / vec2(textureSize(u_texture, 0));
	vec2 screenTexCoords = gl_FragCoord.xy * texelSize;
	vec2 blurVec = texture(u_velocityBuffer, screenTexCoords).xy * u_blurPower;

	int n = u_blurLevel;

	dvec4 result = dvec4(0, 0, 0, 0);

	double e = 0.00001;

	if (abs(blurVec.x) > e || abs(blurVec.y) > e)
	{
		for (int i = n; i > 0; i--)
		{
			vec2 offset;
			if (n > 1)
				offset = -blurVec * (float(i) / float(n) - 0.5);
			else
				offset = vec2(0, 0);

			result += dvec4(texture(u_texture, vs_textureCoord + offset)) * double(i) * 2 / double(n * n + n);
		}
	}
	else
	{
		result = dvec4(texture(u_texture, vs_textureCoord));
	}

	color = vec4(result);
}