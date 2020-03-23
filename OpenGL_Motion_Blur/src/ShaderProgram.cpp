#include "ShaderProgram.h"

#include <vector>

ShaderProgram::ShaderProgram(Shader vertexShader, Shader fragmentShader)
{
	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShader.GetShaderID());
	glAttachShader(m_ID, fragmentShader.GetShaderID());
	glLinkProgram(m_ID);
	GLint isLinked = 0;
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_ID);
}

void ShaderProgram::UseProgram()
{
	glUseProgram(m_ID);
}