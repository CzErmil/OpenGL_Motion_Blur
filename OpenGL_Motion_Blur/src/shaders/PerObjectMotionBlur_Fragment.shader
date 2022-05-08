#version 330 core

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

	//float speed = length(blurVec / texelSize);
	//n = clamp(int(speed), 1, 100);

	vec4 result = vec4(0, 0, 0, 0);

	float e = 0.00001;

	if (abs(blurVec.x) > e || abs(blurVec.y) > e)
	{
		for (int i = 0; i < n; i++)
		{
			vec2 offset;
			if (n > 1)
				offset = blurVec * (float(i) / float(n - 1) - 0.5);
			else
				offset = vec2(0, 0);

			result += texture(u_texture, vs_textureCoord + offset);
		}
		result /= float(n);
	}
	else
	{
		result = texture(u_texture, vs_textureCoord);
	}

	color = result;
}