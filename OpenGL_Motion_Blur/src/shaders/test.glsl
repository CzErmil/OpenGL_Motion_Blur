#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureCoord;

uniform mat4 u_MVP;
uniform mat4 u_Model;
uniform mat4 u_prevMVP;

out vec3 vs_normal;
out vec2 vs_textureCoord;
out vec3 vs_fragCoord;
out vec3 color;

void main(void)
{
	vec4 pos = u_MVP * position;
	vec4 prevPos = u_prevMVP * position;

	vec4 velVec = pos - prevPos;

	float cosAlfa = dot( normalize(vec3(velVec)) ,normal );

	vec4 newPos = pos + velVec * cosAlfa;
	
	gl_Position = newPos;

	vs_normal = vec3(mat3(u_Model) * normal);
	vs_textureCoord = textureCoord;
	vs_fragCoord = vec3(u_Model * position);
}