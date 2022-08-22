#version 330 core

smooth in vec4 vs_prevPos;
smooth in vec4 vs_Pos;

out vec2 velocity;

void main(void)
{
	vec2 a = (vs_Pos.xy / vs_Pos.w) * 0.5 + 0.5;
	vec2 b = (vs_prevPos.xy / vs_prevPos.w) * 0.5 + 0.5;
	velocity = a - b;
}