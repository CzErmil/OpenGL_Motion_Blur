#include "ShaderProgram.h"

#include <vector>

ShaderProgram::ShaderProgram(Shader vertexShader, Shader fragmentShader)
{
	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertexShader.GetShaderID());
	glAttachShader(m_ID, fragmentShader.GetShaderID());
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

GLint ShaderProgram::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	
	GLint location = glGetUniformLocation(m_ID, name.c_str());
	if (location == -1)
	{
		std::cout << "Uniform \""<< name <<"\" doesn't exist!\n";
	}
	
	m_UniformLocationCache[name] = location;

	return location;
}

void ShaderProgram::SetUniform1i(const std::string& name, GLuint x1)
{
	glUniform1i(GetUniformLocation(name), x1);
}

void ShaderProgram::SetUniform1f(const std::string& name, GLfloat x1)
{
	glUniform1f(GetUniformLocation(name), x1);
}

void ShaderProgram::SetUniform2f(const std::string& name, GLfloat x1, GLfloat x2)
{
	glUniform2f(GetUniformLocation(name), x1, x2);
}

void ShaderProgram::SetUniform3f(const std::string& name, GLfloat x1, GLfloat x2, GLfloat x3)
{
	glUniform3f(GetUniformLocation(name), x1, x2, x3);
}

void ShaderProgram::SetUniform4f(const std::string& name, GLfloat x1, GLfloat x2, GLfloat x3, GLfloat x4)
{
	glUniform4f(GetUniformLocation(name), x1, x2, x3, x4);
}

void ShaderProgram::SetUniformMatrix4fv(const std::string& name, glm::mat4 m)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1,GL_FALSE, &m[0][0]);
}