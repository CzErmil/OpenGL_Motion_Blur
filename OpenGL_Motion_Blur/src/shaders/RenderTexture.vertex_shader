#version 330 core

out vec2 vs_textureCoord;

void main(void)
{

	const vec4 positions[6] = vec4[6](	vec4(-1.0, -1.0, 0.0, 1.0),
										vec4( 1.0, -1.0, 0.0, 1.0),
										vec4( 1.0,  1.0, 0.0, 1.0),
										vec4(-1.0, -1.0, 0.0, 1.0),
										vec4( 1.0,  1.0, 0.0, 1.0),
										vec4(-1.0,  1.0, 0.0, 1.0));

	const vec2 textureCoords[6] = vec2[6](	vec2(0.0, 0.0),
											vec2(1.0, 0.0),
											vec2(1.0, 1.0),
											vec2(0.0, 0.0),
											vec2(1.0, 1.0),
											vec2(0.0, 1.0));

	gl_Position = positions[gl_VertexID];

	vs_textureCoord = textureCoords[gl_VertexID];
}