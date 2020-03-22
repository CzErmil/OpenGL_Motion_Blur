#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(Shader vertexShader, Shader fragmentShader)
{
	m_ID = glCreateProgram();
	glAttachShader(vertexShader.GetShaderID(), vertexShader.GetShaderType());
	glAttachShader(fragmentShader.GetShaderID(), fragmentShader.GetShaderType());
	glLinkProgram(m_ID);
}

ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(m_ID);
}

void ShaderProgram::UseProgram()
{
	glUseProgram(m_ID);
}