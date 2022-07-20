#version 410 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;

out vec3 vs_normal;
out vec2 vs_textureCoord;
out int vs_instanceID;

void main(void)
{
	gl_Position = position;

	vs_normal = normal;
	vs_textureCoord = textureCoord;
	vs_instanceID = gl_InstanceID;
}