#version 400 core

in vec2 vs_textureCoord;

uniform sampler2D u_texture;
uniform sampler2D u_depthMap;
uniform mat4 u_invMVP;
uniform mat4 u_prevMVP;
uniform int u_BlurLevel;
uniform float u_BlurPower;

out vec4 color;

void main(void)
{
	if(texture(u_depthMap, vs_textureCoord).x==1)
	{
		color = texture(u_texture, vs_textureCoord);
		return;
	}

	vec4 curentPos = vec4((vs_textureCoord.xy - 0.5) * 2, texture(u_depthMap, vs_textureCoord).x,1.0f);
	vec4 invPos = u_invMVP * curentPos;
	vec4 prevPos = u_prevMVP * invPos;
	prevPos = vec4((prevPos.xy * 0.5) + 0.5, prevPos.zw);

	vec2 blurVec = prevPos.xy - vs_textureCoord;

	blurVec *= u_BlurPower;

	int n = u_BlurLevel;

	dvec4 result = dvec4(0, 0, 0, 0);

	for (int i = n; i > 0; i--)
	{
		vec2 offset = blurVec * (float(i) / float(n) - 0.5);

		double power = double(i) * 2 / double(n * n + n);

		if((vs_textureCoord.x + offset.x) > 1 || (vs_textureCoord.x + offset.x) < 0 || (vs_textureCoord.y + offset.y) > 1 || (vs_textureCoord.y + offset.y) < 0)
		{
			result += texture(u_texture, vs_textureCoord) * power;
		}
		else
		{
			result += texture(u_texture, vs_textureCoord + offset) * power;
		}
	}

	color = vec4(result);
}