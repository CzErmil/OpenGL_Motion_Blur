#version 330 core

in vec2 vs_textureCoord;

uniform sampler2D u_texture;
uniform sampler2D u_depthMap;
uniform mat4 u_invMVP;
uniform mat4 u_prevMVP;
uniform int u_BlurLevel;

out vec4 color;

void main(void)
{
	vec4 curentPos = vec4((vs_textureCoord.xy - 0.5) * 2, texture(u_depthMap, vs_textureCoord).x,1.0f);
	vec4 invPos = u_invMVP * curentPos;
	vec4 prevPos = u_prevMVP * invPos;
	prevPos = vec4((prevPos.xy * 0.5) + 0.5, prevPos.zw);

	vec2 blurVec = prevPos.xy - vs_textureCoord;

	int n = u_BlurLevel;

	vec4 result = vec4(0, 0, 0, 0);

	for (int i = 0; i < n; i++)
	{
		vec2 offset = blurVec * (float(i) / float(n - 1) - 0.5);

		result += texture(u_texture, vs_textureCoord + offset);
	}

	result /= float(n);

	color = result;
}