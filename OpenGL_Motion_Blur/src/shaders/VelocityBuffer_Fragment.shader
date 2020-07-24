#version 330 core

smooth in vec4 vs_prevPos;
smooth in vec4 vs_Pos;

out vec4 velocity;

void main(void)
{
	vec2 a = vs_Pos.xy * 0.5 + 0.5;
	vec2 b = vs_prevPos.xy * 0.5 + 0.5;
	velocity = vec4(vec2(a-b),0,1);
}