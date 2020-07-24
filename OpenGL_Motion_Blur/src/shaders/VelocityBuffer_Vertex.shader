#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_prevMVP;
uniform mat4 u_MVP;

smooth out vec4 vs_prevPos;
smooth out vec4 vs_Pos;

void main(void)
{
	vs_prevPos = u_prevMVP * position;
	gl_Position = vs_Pos = u_MVP * position;
}