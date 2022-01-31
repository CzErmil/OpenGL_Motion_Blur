#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_prevMVP;
uniform mat4 u_MVP;

smooth out vec4 vs_prevPos;
smooth out vec4 vs_Pos;
out vec4 vs_color;

void main(void)
{
	vs_prevPos = u_prevMVP * position;
	vs_Pos = u_MVP * position;
	vec4 velVec = vs_Pos - vs_prevPos;
	gl_Position = vs_Pos;// +velVec * dot(normalize(vec3(velVec)), normal);
}