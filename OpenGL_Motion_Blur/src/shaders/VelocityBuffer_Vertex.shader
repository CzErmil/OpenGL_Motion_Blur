#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;

uniform mat4 u_prevMVP;
uniform mat4 u_MVP;

smooth out vec4 vs_prevPos;
smooth out vec4 vs_Pos;

void main(void)
{
	vs_prevPos = u_prevMVP * position;
	vs_Pos = u_MVP * position;
	gl_Position = vs_Pos;
}