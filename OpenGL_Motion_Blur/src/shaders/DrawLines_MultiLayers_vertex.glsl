#version 330 core

layout (location = 0) in vec4 position;

uniform vec3 u_color;

out vec3 vs_color;
out int vs_instanceID;

void main(void)
{
	gl_Position = position;

	vs_color = u_color;
	vs_instanceID = gl_InstanceID;
}